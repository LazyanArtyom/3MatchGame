#ifndef MATCH3_ERRORMANAGER_HPP_
#define MATCH3_ERRORMANAGER_HPP_

#include <iostream>

namespace match3 {

	/**
	* \brief ErrorManager class for printing error msg and exit.
	*/
	class ErrorManager final 
	{
	public:
		static ErrorManager& print(const std::string& fileName, const std::string& msg)
		{
			std::cerr << "Error in " << fileName << ": " << msg << std::endl;
			exit(EXIT_FAILURE);
		}

	private:
		ErrorManager() = delete;
		~ErrorManager() = delete;
		ErrorManager(const ErrorManager&) = delete;
		void operator=(const ErrorManager&) = delete;
		ErrorManager(ErrorManager&&) noexcept = default;
		ErrorManager& operator=(ErrorManager&&) noexcept = default;
	};

} // end namespace match3
#endif // MATCH3_ERRORMANAGER_HPP_