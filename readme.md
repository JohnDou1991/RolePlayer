RolePlayer - a Google RPC Demo

[Purpose]
    Learn gRPC practically

[Description]
    In ./bin folder you will find:
    Script.txt - contains dialogue in roles (2 in example).
    Server.exe - reads the script, implements rpc service, 
                 starts to listen to the specified via cmd arguments ip
                 and waits for roles to be assigned.
    Client.exe - uses rpc stub, connects to the server and gains some role from server.
    Demo.exe - starts server, starts clients and assigns the roles to each one of theme.
    Then the scene is being played by server, it distributes lines among clients so that
    clients play their respective roles.
    Note: all binaries share the same std out.

[Requirements]
Windows:
    visual studio 2019
    cmake 3.27

[Build]
Windows:
    mkdir ./build
    cd ./build
    cmake ../
    cmake --build . --config Release

[Run]
Windows:
    ./bin/Release/Demo.exe
