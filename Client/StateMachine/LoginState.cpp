#include "LoginState.h"

LoginState::LoginState(sf::RenderWindow& window, StateMachine& sm, NetClient& netClient)
    : window(window),
      stateMachine(sm),
      font(),
      title(font, ""),
      username_text(font, ""), password_text(font, ""), sign_in_text(font, "Sign in"),
      username_lable(font, "Username"), password_lable(font, "Password"), error_lable(font, ""), registration_lable(font, "Register"),
      netClient(netClient)
{
    if (!font.openFromFile("arial.ttf"))
    {
        throw std::runtime_error("Failed to load font");
    }

    title.setFont(font);
    title.setString("Login");
    title.setCharacterSize(24);
    title.setFillColor(sf::Color::White);
    title.setPosition({ 375, 50 });

    registration_lable.setCharacterSize(16);
    registration_lable.setFillColor({ 85, 164, 248 });
    registration_lable.setPosition({ 375, 300 });
    registration_lable.setStyle(sf::Text::Style::Underlined);

    username_lable.setCharacterSize(18);
    username_lable.setFillColor(sf::Color::White);
    username_lable.setPosition({ 250, 100 });

    password_lable.setCharacterSize(18);
    password_lable.setFillColor(sf::Color::White);
    password_lable.setPosition({ 250, 180 });

    error_lable.setCharacterSize(18);
    error_lable.setFillColor(sf::Color::Red);
    error_lable.setPosition({ 300, 280 });

    sign_in_box.setFillColor(sf::Color::White);
    sign_in_box.setPosition({ 375, 240 });
    sign_in_box.setSize({ 75, 30 });

    username_box.setFillColor(sf::Color::White);
    username_box.setPosition({ 250, 120 });
    username_box.setSize({ 300.f, 30.f });

    password_box.setSize({ 300, 30 });
    password_box.setFillColor(sf::Color::White);
    password_box.setPosition({ 250, 200 });


    username_text.setFont(font);
    username_text.setCharacterSize(18);
    username_text.setFillColor(sf::Color::Black);
    username_text.setPosition({ 250, 120 });

    password_text.setFont(font);
    password_text.setCharacterSize(18);
    password_text.setFillColor(sf::Color::Black);
    password_text.setPosition({ 250, 200 });

    sign_in_text.setCharacterSize(18);
    sign_in_text.setPosition({ 375, 240 });
    sign_in_text.setFillColor(sf::Color::Black);

    is_username_selected = false;
    is_password_selected = false;
    is_sign_in_selected = false;
    is_sign_in_submitted = false;
    is_register_selected = false;
}

void LoginState::handleEvent(const sf::Event& event)
{
    if (const auto* mouseButtonPressed = event.getIf<sf::Event::MouseButtonPressed>())
    {
        sf::Vector2f mouserPos = window.mapPixelToCoords(sf::Mouse::getPosition(window));

        is_username_selected = username_box.getGlobalBounds().contains(mouserPos);
        is_password_selected = password_box.getGlobalBounds().contains(mouserPos);
        is_sign_in_selected = sign_in_box.getGlobalBounds().contains(mouserPos);
        is_register_selected = registration_lable.getGlobalBounds().contains(mouserPos);

        if (is_register_selected)
        {
            stateMachine.pushState(std::make_unique<RegistrationState>(window, stateMachine, netClient));
        }

        if (is_sign_in_selected && !is_sign_in_submitted)
        {
            if (!username.empty() && !password.empty())
            {
                if (netClient.sendLoginData(username, password))
                {
                    std::string response = netClient.receiveRawResponse();

                    if (response == "LOGIN_SUCCESS")
                    {
                        is_sign_in_submitted = true;
                        stateMachine.pushState(std::make_unique<ChatState>(window, stateMachine, netClient));
                    }
                    else if (response == "ERROR:INVALID_PASSWORD")
                    {
                        error_lable.setString("Invalid username or password.");
                        is_sign_in_submitted = false;
                    }
                    else
                    {
                        error_lable.setString("Unexpected server response.");
                        is_sign_in_submitted = false;
                    }
                }
                else
                {
                    error_lable.setString("Failed to send login data. Please try again.");
                    is_sign_in_submitted = false;
                }
            }
            else
            {
                error_lable.setString("Please fill in all fields.");
                is_sign_in_submitted = false;
            }
        }
    }

    if (const auto* textEntered = event.getIf<sf::Event::TextEntered>())
    {
        char unicode_char = static_cast<char>(textEntered->unicode);

        if (is_username_selected)
        {
            if (textEntered->unicode == 8)
            {
                if (!username.empty()) 
                {
                    username.pop_back();
                }
            }
            else if (textEntered->unicode < 128 && std::isprint(unicode_char))
            {
                username += unicode_char;
            }
            username_text.setString(username);
        }
        else if (is_password_selected)
        {
            if (textEntered->unicode == 8)
            {
                if (!password.empty())
                {
                    password.pop_back();
                }
            }
            else if (textEntered->unicode < 128 && std::isprint(unicode_char))
            {
                password += unicode_char;
            }
            password_text.setString(password);
        }
    }
}

void LoginState::update()
{

}

void LoginState::render(sf::RenderWindow& window)
{
    window.clear({26, 34, 44});
    window.draw(title);
    window.draw(username_lable);
    window.draw(password_lable);
    window.draw(error_lable);
    window.draw(registration_lable);
    window.draw(username_box);
    window.draw(password_box);
    window.draw(sign_in_box);
    window.draw(username_text);
    window.draw(password_text);
    window.draw(sign_in_text);
}
