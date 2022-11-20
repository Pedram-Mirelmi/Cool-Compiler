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
        a <- 
        if(true) then 
            new A 
        else 
            new B fi;
    }};
};

