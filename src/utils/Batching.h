#pragma once

#include <vector>

namespace lightEngine
{

	namespace utils
	{

		template<typename T>
		class BatchList_ptr
		{
		private:

			std::vector<std::vector<T>> _batches;

			bool(*_batchIdentificationFunc)(T, T) = nullptr;

		public:

			BatchList_ptr(bool(*batchIdentificationFunc)(T, T)) :
				_batchIdentificationFunc(batchIdentificationFunc)
			{}

			~BatchList_ptr()
			{}

			/*
				Goes through each batch until "_batchIdentificationFunc" returns true -> after that adds the T into that batch.
				If no suitable batch is found -> creates new batch
			*/
			void insert(T t)
			{
				for (std::vector<T>& batch : _batches)
				{
					if (!batch.empty())
					{
						T batchHead = batch[0];
						if (batchHead)
						{
							if (_batchIdentificationFunc(t, batchHead))
							{
								batch.push_back(t);
								return;
							}
						}
					}
				}

				createNewBatch(t);
			}

			void clear()
			{
				_batches.clear();
			}

			std::vector<T>& getBatch(unsigned int index)
			{
				return _batches[index];
			}

			inline std::vector<std::vector<T>>& getAllBatches() { return _batches; }


			void createNewBatch(T t)
			{
				std::vector<T> newBatch({ t });
				_batches.push_back(newBatch);
			}
		};
	}
}