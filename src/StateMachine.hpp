#ifndef MATCH3_STATEMACHINE_HPP_
#define MATCH3_STATEMACHINE_HPP_

#include <stack>
#include <memory>

#include "State.hpp"

namespace match3 {
	/**
	* \brief StateMachine a class that manages all the stats in the game and
	*/
	class StateMachine
	{
	public:
		using StatePtrType = std::unique_ptr<impl::IState>;
		using StatePtrCollectionType = std::stack<StatePtrType>;

	public:
		StateMachine() = default;
		~StateMachine() = default;

		void removeState();
		void addState(StatePtrType newState, bool isReplacing = true);

		// run at start of each loop in GameController.cpp
		void processStateChanges();
		StatePtrType& getActiveState();

	private:
		StatePtrType newState_;
		StatePtrCollectionType states_;

		bool isAdding_ = false;
		bool isRemoving_ = false;
		bool isReplacing_ = false;
	};

} // end namespace match3
#endif // MATCH3_STATEMACHINE_HPP_