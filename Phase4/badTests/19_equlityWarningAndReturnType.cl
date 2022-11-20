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
    f(x: Int, y: Int) : String
    {{
        if(a = 1) then 
            new String 
        else 
            new Int 
        fi;
    }};
};

