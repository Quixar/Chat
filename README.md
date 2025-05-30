- În folderul Client există un folder StateMachine care are o clasă abstractă de bază State.  
- În clasa State, funcțiile handleEvent, update, render sunt virtuale pure.  
- Clasele LoginState, RegistrationState, ChatState moștenesc din clasa State.  
- În folderul Server există un folder numit Entities, acesta conține clasa DataContext, care conține excepții.  
- Clasele ChatState, LoginState, RegistrationState folosesc static_cast în metoda handleEvent.  
- S-a folosit și design pattern „State”.  
- Clasa State este o interfață care este implementată de clasele LoginState, RegistrationState, ChatState, precum și de clasa StateMachine, care este responsabilă pentru schimbarea stărilor.  
- Clasa DataContext are o funcție șablon care este responsabilă pentru înregistrarea informațiilor pe consola serverului.  

