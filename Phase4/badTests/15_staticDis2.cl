class A 
{
    a: String;
    f(x: Int, y: Int) : Object
    {{
        a;
    }};
};

class B inherits A
{
    f(x: Int) : Object
    {{
        self@A.f(x);
    }};
};
