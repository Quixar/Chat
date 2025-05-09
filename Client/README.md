# Chat Client

## Descriere

Acest proiect reprezintă partea client a unui chat, care utilizează biblioteca **SFML** pentru crearea interfeței grafice și conectarea la server prin protocolul **TCP**, cu utilizarea **MySQL** pentru stocarea datelor utilizatorilor. Clientul interacționează cu serverul pentru înregistrare, autentificare și comunicare în timp real.

## Descrierea claselor

### StateMachine

Mecanismul de gestionare a stărilor în client. Se utilizează **patronul de design Stare** (State Pattern) pentru gestionarea tranzițiilor între diferitele stări ale aplicației, cum ar fi `LoginState`, `RegistrationState` și `ChatState`. Acest patron permite gestionarea convenabilă a stărilor și logicii asociate fiecărei stări, precum și adăugarea ușoară de noi stări în aplicație.

### LoginState

Reprezintă starea pentru autentificarea utilizatorului prin introducerea unui nume de utilizator și parolă.

### RegistrationState

Stare pentru înregistrarea unui utilizator nou. Include o interfață pentru introducerea datelor.

## Dependențe

- **SFML** — pentru interfața grafică. Este folosit pentru crearea ferestrelor, gestionarea evenimentelor, redarea graficelor și interacțiunea rețelei.
- **MySQL Connector/C++** — pentru conectarea și lucrul cu baza de date MySQL.
- **C++17** — pentru compilarea și construirea proiectului.
- **Pattern Stare** — pattern de design utilizat pentru gestionarea stărilor aplicației. Este folosit pentru gestionarea tranzițiilor între stările `LoginState`, `RegistrationState`.

## Bibliografie (Partea Client)

- [Refactoring Guru](https://refactoring.guru)
- [Documentația SFML 3.0.1](https://www.sfml-dev.org/documentation/3.0.1/)
- [Documentația MySQL Connector/C++](https://dev.mysql.com/doc/dev/connector-cpp/latest/)
- [Documentația C++](https://cplusplus.com)
- [ChatGPT](https://chatgpt.com)
