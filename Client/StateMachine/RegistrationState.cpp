#include "RegistrationState.h"

RegistrationState::RegistrationState(sf::RenderWindow& win, StateMachine& sm, NetClient& netClient)
    : window(win), 
      stateMachine(sm),
      font(),
      title(font, ""),
      username_box(), password_box(), email_box(), sign_up_box(),
      username_text(font, ""), password_text(font, ""), email_text(font, ""),
      username_lable(font, "Username"), password_lable(font, "Password"), email_lable(font, "Email"), sign_up_text(font, "Sign up"), error_lable(font, ""), login_lable(font, "Login"),
      netClient(netClient)
{
    if (!font.openFromFile("arial.ttf")) 
    {
        throw std::runtime_error("Failed to load font");
    }

    title.setFont(font);
    title.setString("Registration");
    title.setCharacterSize(24);
    title.setFillColor(sf::Color::White);
    title.setPosition({350, 50});

    login_lable.setCharacterSize(16);
    login_lable.setFillColor({ 85, 164, 248 });
    login_lable.setStyle(sf::Text::Style::Underlined);
    login_lable.setPosition({ 375, 380 });

    username_lable.setCharacterSize(18);
    username_lable.setFillColor(sf::Color::White);
    username_lable.setPosition({ 250, 120 });

    password_lable.setCharacterSize(18);
    password_lable.setFillColor(sf::Color::White);
    password_lable.setPosition({ 250, 180 });

    email_lable.setCharacterSize(18);
    email_lable.setFillColor(sf::Color::White);
    email_lable.setPosition({ 250, 240 });

    error_lable.setCharacterSize(18);
    error_lable.setFillColor(sf::Color::Red);
    error_lable.setPosition({ 300, 340 });

    username_box.setFillColor(sf::Color::White);
    username_box.setPosition({250, 140});
    username_box.setSize({300, 30 });

    password_box.setFillColor(sf::Color::White);
    password_box.setPosition({250, 200 });
    password_box.setSize({300, 30});

    email_box.setFillColor(sf::Color::White);
    email_box.setPosition({250, 260});
    email_box.setSize({300, 30});

    sign_up_box.setFillColor(sf::Color::White);
    sign_up_box.setPosition({ 375, 300 });
    sign_up_box.setSize({ 75, 30 });

    username_text.setFont(font);
    username_text.setCharacterSize(18);
    username_text.setPosition({ 250, 140 });
    username_text.setFillColor(sf::Color::Black);

    password_text.setFont(font);
    password_text.setCharacterSize(18);
    password_text.setPosition({ 250, 200  });
    password_text.setFillColor(sf::Color::Black);

    email_text.setFont(font);
    email_text.setCharacterSize(18);
    email_text.setPosition({ 250, 260 });
    email_text.setFillColor(sf::Color::Black);

    sign_up_text.setCharacterSize(18);
    sign_up_text.setPosition({ 375, 300 });
    sign_up_text.setFillColor(sf::Color::Black);
    
    is_password_selected = false;
    is_email_selected = false;
    is_username_selected = false;
    is_registration_submitted = false;
    is_sign_up_selected = false;
    is_login_selected = false;
}

void RegistrationState::handleEvent(const sf::Event& event) 
{
    
    if (const auto* mouseButtonPressed = event.getIf<sf::Event::MouseButtonPressed>()) 
    {
        sf::Vector2f mousePos = window.mapPixelToCoords(sf::Mouse::getPosition(window));

        is_username_selected = username_box.getGlobalBounds().contains(mousePos);
        is_password_selected = password_box.getGlobalBounds().contains(mousePos);
        is_email_selected    = email_box.getGlobalBounds().contains(mousePos);
        is_sign_up_selected = sign_up_box.getGlobalBounds().contains(mousePos);
        is_login_selected = login_lable.getGlobalBounds().contains(mousePos);

        if (is_login_selected)
        {
            stateMachine.pushState(std::make_unique<LoginState>(window, stateMachine, netClient));
        }

        if (is_sign_up_selected && !is_registration_submitted)
        {
            if (!username.empty() && !password.empty() && !email.empty())
            {
                if (netClient.sendRegistrationData(username, password, email))
                {
                    std::string response = netClient.receiveRawResponse();

                    if (response == "REG_SUCCESS")
                    {
                        is_registration_submitted = true;
                        stateMachine.pushState(std::make_unique<ChatState>(window, stateMachine, netClient));
                    }
                    else if (response == "ERROR:USERNAME_EXISTS")
                    {
                        error_lable.setString("Username already exists. Try another one.");
                        is_registration_submitted = false;
                    }
                    else if (response == "ERROR:INVALID_DATA")
                    {
                        error_lable.setString("Invalid data. Please check your input.");
                        is_registration_submitted = false;
                    }
                    else
                    {
                        error_lable.setString("Unexpected server response.");
                        is_registration_submitted = false;
                    }
                }
                else
                {
                    error_lable.setString("Failed to send registration data. Please try again.");
                    is_registration_submitted = false;
                }
            }
            else
            {
                error_lable.setString("Please fill in all fields.");
                is_registration_submitted = false;
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
                if (!username.empty()) username.pop_back();
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
                if (!password.empty()) password.pop_back();
            }
            else if (textEntered->unicode < 128 && std::isprint(unicode_char))
            {
                password += unicode_char;
            }
            password_text.setString(password);
        }
        else if (is_email_selected) 
        {
            if (textEntered->unicode == 8)
            {
                if (!email.empty()) email.pop_back();
            }
            else if (textEntered->unicode < 128 && std::isprint(unicode_char))
            {
                email += unicode_char;
            }
            email_text.setString(email);
        }
    }
}

void RegistrationState::update() 
{
    
}

void RegistrationState::render(sf::RenderWindow& window) 
{
    window.clear({ 26, 34, 44 });
    window.draw(title);
    window.draw(username_lable);
    window.draw(password_lable);
    window.draw(error_lable);
    window.draw(email_lable);
    window.draw(login_lable);
    window.draw(username_box);
    window.draw(password_box);
    window.draw(email_box);
    window.draw(sign_up_box);
    window.draw(username_text);
    window.draw(password_text);
    window.draw(email_text);
    window.draw(sign_up_text);
}
