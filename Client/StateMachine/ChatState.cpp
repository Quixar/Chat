#include "ChatState.h"

ChatState::ChatState(sf::RenderWindow& window, StateMachine& sm, NetClient& client)
    : window(window), stateMachine(sm), client(client), inputDisplay(font), sendButtonText(font)
{
    if (!font.openFromFile("arial.ttf")) {
        std::cerr << "Failed to load font.\n";
    }

    float panelWidth = 250.f;
    float inputHeight = 30.f;
    float padding = 10.f;
    float buttonWidth = 80.f;

    inputBox.setSize({window.getSize().x - panelWidth - buttonWidth - 3 * padding, inputHeight});
    inputBox.setPosition({panelWidth + padding, window.getSize().y - inputHeight - padding});
    inputBox.setFillColor(sf::Color(50, 50, 50));

    inputDisplay.setFont(font);
    inputDisplay.setCharacterSize(18);
    inputDisplay.setFillColor(sf::Color::White);
    inputDisplay.setPosition({inputBox.getPosition().x + 5.f, inputBox.getPosition().y + 3.f});

    sendButton.setSize({80.f, 30.f});
    sendButton.setPosition({inputBox.getPosition().x + inputBox.getSize().x + 10.f, inputBox.getPosition().y});
    sendButton.setFillColor(sf::Color(70, 130, 180)); 

    sendButtonText.setFont(font);
    sendButtonText.setCharacterSize(18);
    sendButtonText.setFillColor(sf::Color::White);
    sendButtonText.setString("Send");
    sendButtonText.setPosition({
        sendButton.getPosition().x + 15.f,
        sendButton.getPosition().y + 3.f}
    );
    update();
}

void ChatState::handleEvent(const sf::Event& event) 
{
    if (const auto* mouseButtonPressed = event.getIf<sf::Event::MouseButtonPressed>())
    {
        sf::Vector2f mousePos = window.mapPixelToCoords(sf::Mouse::getPosition(window));
        handleMouseClick(mousePos);

        if (sendButton.getGlobalBounds().contains(mousePos)) {
            sendMessage();
        }
    }
    else if (const auto* textEntered = event.getIf<sf::Event::TextEntered>())
    {
        if (selectedUserIndex >= 0)
        {
            if (textEntered->unicode == 8) { 
                if (!inputText.empty()) {
                    inputText.pop_back();
                }
            }
            else if (textEntered->unicode < 128 && textEntered->unicode >= 32) { 
                inputText += static_cast<char>(textEntered->unicode);
            }

            inputDisplay.setString(inputText);
        }
    }
}

void ChatState::update() {
    if (updateClock.getElapsedTime().asSeconds() >= updateInterval)
    {
        fetchActiveUsers();
        if (selectedUserIndex >= 0 && selectedUserIndex < (int)activeUsers.size()) 
        {
            std::string chatPartner = activeUsers[selectedUserIndex];
            std::cout << "[DEBUG] Checking for new messages from " << chatPartner << "\n";

            if (client.hasNewMessagesFrom(chatPartner)) 
            {
                std::cout << "[DEBUG] New messages found, fetching...\n";
                auto newMessages = client.fetchMessagesFrom(chatPartner);
                for (const auto& msg : newMessages) 
                {
                    chatMessages.push_back(msg);
                }
            } 
            else 
            {
                std::cout << "[DEBUG] No new messages.\n";
            }
        }
        updateClock.restart();
    }

}

void ChatState::render(sf::RenderWindow& window) {
    window.clear(sf::Color::White); 
    renderUserList();
    renderChatWindow();
}

void ChatState::fetchActiveUsers() {
    std::string rawResponse = client.getActiveUsers();

    activeUsers.clear();
    userEntries.clear();

    std::stringstream ss(rawResponse);
    std::string username;

    while (std::getline(ss, username, '|'))
    {
        if (!username.empty())
        {
            activeUsers.push_back(username);
        }
    }

    userEntries.clear();
    float entryHeight = 50.f;

    for (size_t i = 0; i < activeUsers.size(); ++i) {
        float y = i * entryHeight;
        userEntries.emplace_back(activeUsers[i], font, y);
    }
}

void ChatState::renderUserList() {
    for (size_t i = 0; i < userEntries.size(); ++i) {
        auto& entry = userEntries[i];
        entry.background.setFillColor(i == selectedUserIndex ? sf::Color(72, 105, 147) : sf::Color(26, 34, 44));
        entry.username.setFillColor(sf::Color::White);
        window.draw(entry.background);
        window.draw(entry.username);
    }
}

void ChatState::renderChatWindow() {
    float panelWidth = 250.f;

    sf::RectangleShape chatArea;
    chatArea.setPosition({panelWidth, 0});
    chatArea.setSize({static_cast<float>(window.getSize().x) - panelWidth, static_cast<float>(window.getSize().y)});
    chatArea.setFillColor(sf::Color(26, 34, 44));
    chatArea.setOutlineColor(sf::Color(200, 200, 200));
    chatArea.setOutlineThickness(1.f);
    window.draw(chatArea);

    if (selectedUserIndex >= 0) {
        sf::Text chatHeader(font);
        chatHeader.setFont(font);
        chatHeader.setCharacterSize(20);
        chatHeader.setFillColor(sf::Color::White);
        chatHeader.setString("Chat cu " + activeUsers[selectedUserIndex]);
        chatHeader.setPosition({panelWidth + 10, 10});
        window.draw(chatHeader);

        float msgStartY = 50.f;
        float lineSpacing = 22.f;
        size_t maxMessages = (window.getSize().y - 100) / lineSpacing;

        size_t startIndex = 0;
        if (chatMessages.size() > maxMessages)
            startIndex = chatMessages.size() - maxMessages;

        for (size_t i = startIndex; i < chatMessages.size(); ++i) {
            sf::Text msgText(font);
            msgText.setFont(font);
            msgText.setCharacterSize(18);
            msgText.setFillColor(sf::Color::White);
            msgText.setString(chatMessages[i]);
            msgText.setPosition({panelWidth + 10, msgStartY + (i - startIndex)*lineSpacing});
            window.draw(msgText);
        }

        window.draw(inputBox);
        window.draw(sendButton);
        window.draw(sendButtonText);
        window.draw(inputDisplay);
    }
}

void ChatState::handleMouseClick(const sf::Vector2f& mousePos) {
    float entryHeight = 50.f;
    float panelWidth = 250.f;

    if (mousePos.x > panelWidth)
        return;

    int index = static_cast<int>(mousePos.y / entryHeight);
    if (index >= 0 && index < static_cast<int>(activeUsers.size())) {
        selectedUserIndex = index;
        chatMessages.clear();
    }
}

void ChatState::sendMessage() {
    if (selectedUserIndex < 0 || inputText.empty())
        return;

    std::string receiver = activeUsers[selectedUserIndex];

    if (client.sendMessage(receiver, inputText)) {
        chatMessages.push_back("You: " + inputText);
        inputText.clear();
        inputDisplay.setString(inputText);
    }
    else {
        std::cerr << "Failed to send message\n";
    }
}

