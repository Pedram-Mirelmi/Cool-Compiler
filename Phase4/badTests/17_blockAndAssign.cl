class A 
{
    a: String;
    f(x: Int) : Object
    {{
        a;
    }};
};

class B inherits A
{
    f(x: Int, y: Int) : Object
    {{
        a <- {"hiii"; 1;};
    }};
};

