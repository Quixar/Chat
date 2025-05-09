# Chat Server 

## Descriere

Serverul este implementat in **C++** si gestioneaza conexiunile **TCP** primite de la clienti. Acesta ofera funcionalitati de inregistrare, autentificare si in viitor trimiterea de mesaje intre utilizatori.

Pentru stocarea datelor se foloseste **MySQL**, iar accessul la baza de date se realizeaza prin **MySQL Connector/C++**.

---

## Componente

- **'Server'**
  Serverul TCP principal, care accepta conexiuni si redictioneaza cererile primite.

- **'DataContext'**
  Clasa care reprezinta un utilizator, incluzand detalii precum id-ul, numele, parola criptata si rolul utilizatorului.

- **'User'**
  Clasa care reprezinta un utilizator, incluzand detalii precum id-ul, numele, parola criptata si rolul utilizatorului.

- **'UserRole'**
  Clasa care defineste gi gestioneaza rolurile utilizatorilor, cum ar fi 'User', si 'Admin'.

---

## Dependente

- **C++17**
- **MySQL Connector/C++ 8.x**
- **SFML**
- CMake

## Bibliografie si surse

- [Refactoring Guru](https://refactoring.guru)
- [SFML Documentation 3.0.1](https://www.sfml-dev.org/documentation/3.0.1/)
- [MySQL Connector/C++ Documentation](https://dev.mysql.com/doc/dev/connector-cpp/latest/)
- [C++ Documentation](https://cplusplus.com)
- [ChatGPT](https://chatgpt.com)
