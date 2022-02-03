#include "StateMachine.hpp"

namespace match3 {
	
	void StateMachine::removeState()
	{
		isRemoving_ = true;
	}

	void StateMachine::addState(StatePtrType newState, bool isReplacing)
	{
		isAdding_ = true;
		isReplacing_ = isReplacing;

		newState_ = std::move(newState);
	}


	void StateMachine::processStateChanges()
	{
		if (isRemoving_ && !states_.empty())
		{
			states_.pop();

			if (!states_.empty())
			{
				states_.top()->resume();
			}

			isRemoving_ = false;
		}

		if (isAdding_)
		{
			if (!states_.empty())
			{
				if (isReplacing_)
				{
					states_.pop();
				}
				else
				{
					states_.top()->pause();
				}
			}

			states_.push(std::move(newState_));
			states_.top()->init();
			isAdding_ = false;
		}
	}

	StateMachine::StatePtrType& StateMachine::getActiveState()
	{
		return states_.top();
	}

} // end namespace match3