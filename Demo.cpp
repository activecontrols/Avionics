class MyClass {
    public:
        int myNum;
        string myString;
};

int main(){
    MyClass myObj;

    myObj.myNum = 15;
    myObj.myString = "Some Text";

    cout << myObj.myNum << "\n";
    cout << myObj.myString;
    return 0;
}