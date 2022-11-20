class A 
{
    a: String;
    f(x: Int) : Object
    {{
        a;
    }};
};

class B
{
    f(x: Int, y: Int) : Object
    {{
        self@A.f(5);
    }};
};

