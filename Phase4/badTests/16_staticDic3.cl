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
        self@A.f(x, y);
    }};
};

