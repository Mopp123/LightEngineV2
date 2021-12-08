
#include "ModelLoading.h"
#include <assimp\scene.h>
#include <assimp\Importer.hpp>
#include <assimp\postprocess.h>
#include "animations\skeletalAnimation\Skeleton.h"
#include "animations\skeletalAnimation\Keyframe.h"
#include "core\Application.h"
#include "core\Debug.h"
#include "core\Common.h"

#include "MeshVertexParsing.h"
#include "MeshSkeletonParsing.h"

/*
	Limitations :

		For all meshes:
		- We support only 2D texture coordinates (No cube mapping..)
		- Currently there can be only 1 diffuse, specular and normal texture per mesh (we cannot have texture blending at the moment..)
		- Currently we support only 1 set of texture coordinates per vertex! because of the above! (stuff in function: "convert_assimp_mesh(aiMesh*)")

		For skinned meshes:
		- Skinned meshes can have only "one" animation
		- Skinned mesh animations NEED TO HAVE keyframes for each bone always at the same time
*/


#define MODEL_LOADING__RESOURCE_FOLDER__TEXTURE		"res/"
#define MODEL_LOADING__RESOURCE_SPECIFIER_MATERIAL	"_material"


namespace lightEngine
{
	namespace graphics
	{
		namespace modelLoading
		{
			// converts assimp mesh to our mesh type
			static Mesh* convert_assimp_mesh(const aiScene* assimpScene, aiMesh* assimpMesh, unsigned int meshBaseIndex, bool loadTangents, bool loadBones, Material* materialToUse = nullptr)
			{
				const bool hasTexCoords = assimpMesh->HasTextureCoords(0);
				const bool hasNormals = assimpMesh->HasNormals();
				const bool hasTangents = loadTangents && assimpMesh->HasTangentsAndBitangents();
				const bool hasBones = loadBones && assimpMesh->HasBones();
				const bool isInstanced = !hasBones; // just quickly for now..

				// Basic vertex data
				std::vector<float> vertexData;
				std::vector<unsigned int> indices;

				load_vertex_data(
					assimpScene, assimpMesh,
					vertexData,
					indices,
					hasTangents,
					meshBaseIndex
				);

				//.. fancier vertex stuff..
				// Skinning data
				std::vector<int> vertexBoneIds;
				std::vector<float> vertexBoneWeights;
				std::vector<animation::Bone*> bones;

				std::map<std::string, unsigned int> boneMapping;

				if (hasBones)
					load_skinning_data(
						assimpScene, assimpMesh,
						boneMapping,
						bones,
						vertexBoneIds,
						vertexBoneWeights,
						meshBaseIndex
					);

				// Create the fat "basic stuff" vertex buffer
				VertexBuffer* vb = VertexBuffer::create(&vertexData[0], vertexData.size(), BufferUsage::StaticDraw);

				// Create layout for this vertex buffer depending on the shit this mesh has..
				// We will always have vertex position obviously..
				std::vector<VertexBufferElement> vb_layoutElements({
					{ ShaderDataType::Float3, "position" }
					});

				if (hasTexCoords)	vb_layoutElements.insert(vb_layoutElements.end(), { ShaderDataType::Float2, "texCoord" });
				if (hasNormals)		vb_layoutElements.insert(vb_layoutElements.end(), { ShaderDataType::Float3, "normal" });
				if (hasTangents)	vb_layoutElements.insert(vb_layoutElements.end(), { ShaderDataType::Float3, "tangent" });

				vb->setLayout({ vb_layoutElements });

				IndexBuffer* ib = IndexBuffer::create(&indices[0], indices.size(), BufferUsage::StaticDraw);

				// Then all custom shit.. instanced buffers, skinning stuff.. etc, depending on what kind of mesh this is..

				// For static meshes we currently are also required to create an "instanced buffer" since everythings rendered instanced currently..
				if (isInstanced)
				{
					// Buffer for storing transformation matrix per instance..
					VertexBuffer* vb_instanced = VertexBuffer::create(sizeof(float) * 4 * 4 * LE__GRAPHICS__MAX_BATCH_SIZE__STATIC_MESH, BufferUsage::DynamicDraw);

					VertexBufferLayout vb_instancedLayout(
						{
							{ ShaderDataType::Float4, "transformationMatrix_row1", 1 },
							{ ShaderDataType::Float4, "transformationMatrix_row2", 1 },
							{ ShaderDataType::Float4, "transformationMatrix_row3", 1 },
							{ ShaderDataType::Float4, "transformationMatrix_row4", 1 }
						}
					);
					vb_instanced->setLayout(vb_instancedLayout);

					Mesh* mesh = Mesh::create(ib, { vb, vb_instanced }, PrimitiveDrawType::Triangles);
					mesh->setMaterial(materialToUse);

					return mesh;
				}
				if (hasBones)
				{
					// Assemble the skeleton..
					aiNode* assimpRootBone = assimpScene->mRootNode;
					animation::Bone* myRootBone = bones[boneMapping[std::string(assimpRootBone->mName.data)]];

					// Find out the total keyframe count of the animation
					unsigned int keyframeCount = 0;
					for (unsigned int i = 0; i < assimpScene->mAnimations[0]->mNumChannels; i++)
						keyframeCount = std::max(keyframeCount, assimpScene->mAnimations[0]->mChannels[i]->mNumPositionKeys);

					// We need to create root bone's keyframes separately!
					create_bone_keyframes(myRootBone, assimpScene, assimpRootBone, keyframeCount);

					animation::Skeleton* skeleton = new animation::Skeleton();
					skeleton->setRootBone(myRootBone);
					skeleton->setBones(bones);
					skeleton->setBoneMapping(boneMapping);

					assemble_bones(assimpScene, assimpRootBone, boneMapping, keyframeCount, *skeleton, bones);


					// For skinned meshes this setup is a bit different.. Currently skinned meshes cannot be instanced!
					VertexBuffer* vb_vertexBoneIds = VertexBuffer::create(&vertexBoneIds[0], vertexBoneIds.size(), BufferUsage::StaticDraw);
					VertexBuffer* vb_vertexBoneWeights = VertexBuffer::create(&vertexBoneWeights[0], vertexBoneWeights.size(), BufferUsage::StaticDraw);

					VertexBufferLayout vb_vertexBoneIdsLayout({ { ShaderDataType::Int4, "boneIDs" } });
					VertexBufferLayout vb_vertexBoneWeightsLayout({ { ShaderDataType::Float4, "boneWeights" } });

					vb_vertexBoneIds->setLayout(vb_vertexBoneIdsLayout);
					vb_vertexBoneWeights->setLayout(vb_vertexBoneWeightsLayout);

					Mesh* skinnedMesh = Mesh::create(ib, { vb, vb_vertexBoneIds, vb_vertexBoneWeights }, PrimitiveDrawType::Triangles);
					materialToUse->setShader(Shader::get_default_skinned_mesh_shader()); // Change default shader to skinned mesh shader!
					skinnedMesh->setMaterial(materialToUse);
					skinnedMesh->setSkeletonPrototype(skeleton);

					return skinnedMesh;
				}
			}


			// Small helper function that gives us name of an image file (removes all \ and / and other path stuff from it)
			static std::string get_file_name(const std::string& path)
			{
				//		c/images/textures/(NAME START POS)->Test.png
				int nameStartPos = 0;
				for (int i = 0; i < path.length(); i++)
				{
					if (path[i] == '\\' || path[i] == '/')
						nameStartPos = i + 1;
				}

				std::string name;
				for (int i = nameStartPos; i < path.length(); i++)
				{
					if (path[i] != '.')
						name += path[i];
					else
						break;
				}

				return name;
			}

			// Loads mesh's material's texture's image and creates texture INTO our ResourceManager
			static Texture* load_material_texture(aiMaterial* assimpMaterial, aiTextureType textureType)
			{
				aiString pathToImg;
				assimpMaterial->GetTexture(textureType, 0, &pathToImg); // theres 0 since we currently support only 1 texture per vertex
				if (pathToImg.length == 0)
					return nullptr;

				// Figure out name "extension" for creating a texture and image resource
				std::string nameExtension;
				switch (textureType)
				{
				case aiTextureType::aiTextureType_DIFFUSE:	nameExtension = "_diffuse"; break;
				case aiTextureType::aiTextureType_SPECULAR: nameExtension = "_specular"; break;
				case aiTextureType::aiTextureType_NORMALS:	nameExtension = "_normal"; break;
				case aiTextureType::aiTextureType_HEIGHT:	nameExtension = "_height"; break;
				default:
					break;
				}

				std::string imgName = get_file_name(pathToImg.C_Str());
				std::string resourceName = imgName + nameExtension;
				// Check if theres already a texture in the ResourceManager with this name
				//	-> if there is, use that instead!
				Texture* texture = ResourceManager::find_texture(resourceName);

				// If there wasnt already existing texture with this resourceName?
				//	-> Create new Texture resource (and ImgData resource ofc as well..)
				if (!texture)
				{
					std::string finalImagePath = MODEL_LOADING__RESOURCE_FOLDER__TEXTURE + std::string(pathToImg.C_Str());
					ImageData* imgData = ResourceManager::load_resource__image(finalImagePath, resourceName);
					if (!imgData)
					{
						Debug::log(
							"ERROR: Location: ModelLoading: load_material_texture(aiMaterial*, aiTextureType)\n"
							"Failed to load image: " + finalImagePath
						);
						return nullptr;
					}
					return ResourceManager::create_resource__texture(imgData, resourceName, TextureFilteringType::LinearMipmap, TextureImageMapping::ClampToEdge);
				}
				else
				{
#ifdef LIGHT_ENGINE__DEBUG_MODE__FULL
					Debug::log(
						"WARNING: Location: ModelLoading: Texture* load_material_texture(aiMaterial*, aiTextureType)\n"
						"While loading mesh's material's texture we found a texture in the ResourceManager with the same name('" + resourceName + "')\n"
						"	-> We use that texture now instead of loading a new one!"
					);
#endif
					return texture;
				}
			}

			// Creates material according to the aiMesh's material
			// Loads the material's textures specified by the aiMesh's material
			// You need to give "fileName" of the loaded model for this function 
			//		-> we create material resource with ResourceManager here and it needs resource name. 
			//		So we convert the file name into material resource name here!
			// *NOTE Currently we can have only one diffuse, specular and normal texture (no possibility of texture blending at the moment..)
			static Material* load_assimp_material(const aiScene* assimpScene, aiMesh* assimpMesh, const std::string& fileName)
			{
				Texture* texture_diffuse = nullptr;
				Texture* texture_specular = nullptr;
				Texture* texture_normal = nullptr;

				const unsigned int materialIndex = assimpMesh->mMaterialIndex;
				aiMaterial* assimpMaterial = assimpScene->mMaterials[materialIndex];

				texture_diffuse = load_material_texture(assimpMaterial, aiTextureType_DIFFUSE);
				texture_specular = load_material_texture(assimpMaterial, aiTextureType_SPECULAR);
				texture_normal = load_material_texture(assimpMaterial, aiTextureType_NORMALS);
				if (!texture_normal) // apparently some file formats has the normal map as "heightmap".. dunno.. just in case.. this propably fucks up at some point..
					texture_normal = load_material_texture(assimpMaterial, aiTextureType_HEIGHT);


				std::string materialName = assimpMaterial->GetName().C_Str();
				Material* myMaterial = ResourceManager::create_resource__material(
					texture_diffuse,
					texture_specular,
					texture_normal,
					materialName + MODEL_LOADING__RESOURCE_SPECIFIER_MATERIAL
				);

				if (texture_specular)
				{
					float specularStrength = 0.0f;
					float specularShininess = 0.0f;
					aiColor4D assimpSpecularColor;

					if (aiGetMaterialFloat(assimpMaterial, AI_MATKEY_SHININESS_STRENGTH, &specularStrength) != aiReturn_SUCCESS)
					{
						Debug::log(
							"WARNING: Location: ModelLoading: load_assimp_material(const aiScene*, aiMesh*, const std::string&)\n"
							"Failed to get material's 'AI_MATKEY_SHININESS_STRENGTH'\n"
							"(This may happen because all file formats doesn't store this data)"
						);
					}
					if (aiGetMaterialFloat(assimpMaterial, AI_MATKEY_SHININESS, &specularShininess) != aiReturn_SUCCESS)
					{
						Debug::log(
							"WARNING: Location: ModelLoading: load_assimp_material(const aiScene*, aiMesh*, const std::string&)\n"
							"Failed to get material's 'AI_MATKEY_SHININESS'\n"
							"(This may happen because all file formats doesn't store this data)"
						);
					}
					if (aiGetMaterialColor(assimpMaterial, AI_MATKEY_COLOR_SPECULAR, &assimpSpecularColor) != aiReturn_SUCCESS)
					{
						Debug::log(
							"WARNING: Location: ModelLoading: load_assimp_material(const aiScene*, aiMesh*, const std::string&)\n"
							"Failed to get material's 'AI_MATKEY_COLOR_SPECULAR'\n"
							"(This may happen because all file formats doesn't store this data)"
						);
					}

					myMaterial->setSpecularProperties(
						specularStrength,
						specularShininess,
						mml::Vec3(assimpSpecularColor.r, assimpSpecularColor.g, assimpSpecularColor.b)
					);
				}

				// Determine which default shader this material should use..
				// (*Currently it can be either "normal static mesh shader" that has specular and normal mapping, shader with only diffuse lighting or skinned mesh shader)
				// *NOTE -> we may change this later if we want/need to.
				// Skinned meshes get automatically their material's shader to be skinned mesh shader!
				if (texture_specular && texture_normal)
					myMaterial->setShader(Shader::get_default_mesh_shader()); // Shader which supports specular and normal mapping
				else
					myMaterial->setShader(Shader::get_default_simple_mesh_shader()); // Shader which supports only diffuse textures

				return myMaterial;
			}


			// handles an assimp node (handles all child nodes as well 
			//	-> if this is called with root node as "assimpNode" handles every single assimp node we have in here..)
			static void process_assimp_node(Model* model, const aiScene* assimpScene, aiNode* assimpNode, const std::string& fileName, bool loadTangents, bool loadMaterial)
			{
				for (int i = 0; i < assimpNode->mNumMeshes; i++)
				{
					aiMesh* assimpMesh = assimpScene->mMeshes[assimpNode->mMeshes[i]];
					Material* material = nullptr;

					if (loadMaterial)
						material = load_assimp_material(assimpScene, assimpMesh, fileName);

					Mesh* mesh = convert_assimp_mesh(assimpScene, assimpMesh, 0, material->getTexture_normal() != nullptr, assimpMesh->HasBones(), material);

					model->addMesh(mesh);
				}

				for (int i = 0; i < assimpNode->mNumChildren; i++)
					process_assimp_node(model, assimpScene, assimpNode->mChildren[i], fileName, loadMaterial, loadTangents);
			}


			static unsigned int convert_to_assimp_post_process(unsigned int flags)
			{
				unsigned int outFlags = 0;
				if (flags & ModelLoading_PostProcessFlags::FlipUVs)
					outFlags |= aiPostProcessSteps::aiProcess_FlipUVs;
				if (flags & ModelLoading_PostProcessFlags::Triangulate)
					outFlags |= aiPostProcessSteps::aiProcess_Triangulate;
				if (flags & ModelLoading_PostProcessFlags::CalcTangentSpace)
					outFlags |= aiPostProcessSteps::aiProcess_CalcTangentSpace;
				if (flags & ModelLoading_PostProcessFlags::JoinIdenticalVertices)
					outFlags |= aiPostProcessSteps::aiProcess_JoinIdenticalVertices;

				return outFlags;
			}


			Model* load_model(const std::string& path, unsigned int postProcessFlags, bool loadMaterialData)
			{
				Model* model = new Model;

				unsigned int assimpPostProcessFlags = convert_to_assimp_post_process(postProcessFlags);

				Assimp::Importer assimpImporter;
				const aiScene* assimpScene = nullptr;
				assimpScene = assimpImporter.ReadFile(
					path,
					assimpPostProcessFlags
				);
				if (!assimpScene || assimpScene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !assimpScene->mRootNode)
				{
					Debug::log(
						"ERROR: Failed to load model from: " + path + "\n"
						"Assimp error message : " + assimpImporter.GetErrorString()
					);
					Application::exit();
					return model;
				}

				std::string fileName = get_file_name(path);

				process_assimp_node(
					model, assimpScene, assimpScene->mRootNode, fileName, loadMaterialData,
					assimpPostProcessFlags & aiPostProcessSteps::aiProcess_CalcTangentSpace
				);

				//assimpImporter.FreeScene(); // unnecessary, because its called by Importer's destructor?

				return model;
			}
		}
	}
}
