#ifndef WINDOWCHANGE_HPP
#define WINDOWCHANGE_HPP

#include "SFML\Graphics.hpp"


class WindowChange
{
public:
	enum class Type
	{
		CHANGE_WINDOW_SIZE,
		CHANGE_WINDOW_POSITION
	};

	struct Info
	{
	public:
		sf::Vector2u windowSize;
		bool adjustView;

		sf::Vector2i windowPosition;

	public:
		Info() = default;
		Info(sf::Vector2u const & _windowSize, bool _adjustView = true, sf::Vector2i _windowPosition = sf::Vector2i());
		Info(Info const &) = default;
		Info& operator=(Info const &) = default;
		~Info() = default;
	};


public:
	Type type;
	Info info;


public:
	WindowChange() = delete;
	WindowChange(Type _type, Info _info = Info());
	WindowChange(WindowChange const &) = default;
	WindowChange& operator=(WindowChange const &) = default;
	~WindowChange() = default;



};




#endif //WINDOWCHANGE_HPP

