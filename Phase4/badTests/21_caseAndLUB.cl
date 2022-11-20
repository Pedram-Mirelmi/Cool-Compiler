
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
    f(x: Int, y: Int) : B
    {
        case new B of 
            b: B => 
            {
                (new IO).out_string("it's a B");
                b;
            };
            a: A => 
            {
                (new IO).out_string("it's a A"); 
                a;
            };
        esac
    };
};

