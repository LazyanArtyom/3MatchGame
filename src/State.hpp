#ifndef MATCH3_STATE_HPP_
#define MATCH3_STATE_HPP_

namespace match3::impl {
	/**
	* \brief IState is a interface from which all states in the game should inherit
	*/
	class IState
	{
	public:
		virtual ~IState() = default;
		
		virtual void init() = 0;
		virtual void handleInput() = 0;
		virtual void draw() = 0;
		virtual void update() = 0;
		
		// optional
		virtual void pause() { }
		virtual void resume() { }
	};

} // end namespace match3::impl
#endif // MATCH3_STATE_HPP_