# mysql-cpp-connect-test
A small (test)program to demonstate/test the connection to a SQL database using the *ODBC* in C++.

## Description
This C++ program connects to a SQL database and lists all databases.

## Dependencies
The package **unixodbc-dev** is needed. Additionally, all installed installed linux packages are provided for debugging purposes (see */info*). The operating system used was Fedora30 (5.6.13-100.fc30.x86_64).

## Running the program
The following must be done to compile and execute the program:

1. Set the (correct) login credentials in *loginCredentials.hpp*
2. Compile the program using **g++ cppSQLconnectTest.cpp -std=c++17 -lodbc**
3. Execute the program using **./a.out**
