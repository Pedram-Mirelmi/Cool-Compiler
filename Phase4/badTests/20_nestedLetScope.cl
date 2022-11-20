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
    f(x: Int, y: Int) : A
    {{
        let a : Int in 
            let a : String in 
                {a;};
    }};
};

