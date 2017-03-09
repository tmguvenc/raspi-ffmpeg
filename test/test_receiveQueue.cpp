#include <gtest/gtest.h>
#include <receive_strategy_queue.h>
#include <queue>

struct QueueState
{
	
};

struct ArgumentParserTestAll : testing::TestWithParam<QueueState>
{
//	ArgumentParserTestAll()
//	{
//		queue = new ReceiveStrategyQueue<Queue>(&q);
//	}
//
//	virtual ~ArgumentParserTestAll()
//	{
//		delete queue;
//	}
//
//private:
//	using Queue = std::queue<Data*>;
//	Queue q;
//	IReceiveStrategy *queue;
};

int main(int argc, char* argv[])
{
	testing::InitGoogleTest(&argc, argv);
	return RUN_ALL_TESTS();
}
