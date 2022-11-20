class A 
{
    f(a: A) : SELF_TYPE
    {   
        self
    };
};


class B inherits A 
{
    next : B;
    g() : C
    {{
        next <- f(self);
    }};
};

class C {};

