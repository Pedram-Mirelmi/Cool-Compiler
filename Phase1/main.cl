class StackNode
{
	data: String;
    	prev: StackNode;
    	
    init(new_data: String, new_prev: StackNode) : StackNode
    {{
        data <- new_data;
        prev <- new_prev;
        self;
    }};
    
    set_data(new_data: String) : Object
    {{
        data <- new_data;
        1;
    }};
    
    get_data(): String
    {{
        data;
    }};
	
	
    set_prev(new_prev: StackNode) : Object
    {{
        prev <- new_prev;
    }};

    get_prev() : StackNode
    {{
        prev;
    }};



};

class Stack 
{
	top: StackNode;
	temp_val: String;
	push(new_data: String) : Object
    {{
        if (isvoid top) then
        {
            top <- new StackNode;
            top.set_data(new_data);
        }
        else
        {
            top <- (new StackNode).init(new_data, top);
        }
        fi;

    }};
	
    top() : StackNode
    {{
	    top;
    }};

    pop() : String
    {{
	    if(isvoid top) then
        {
            "";
        }
        else
        {
            temp_val <- top.get_data();
            top <- top.get_prev();
            temp_val;
        }
        fi;
    }};
	
    print_stack() : Object
    {{
        let itter: StackNode <- top(),
            io: IO <- (new IO) 
        in 
        {
            while (not (isvoid itter)) loop
            {
                io.out_string(itter.get_data());
                io.out_string("\n");
                itter <- itter.get_prev();
            } pool;
        };
    }};

};


class Main inherits A2I
{
	io: IO <- new IO;
    stack: Stack <- new Stack;
	main() : Object
	{{
	        let input: String <- ""
            in 
            {
                while (not (input = "x")) loop
                {
                    io.out_string("> ");
                    input <- io.in_string();
                    if (input = "d") then
                    {
                        stack.print_stack();
                    }
                    else 
                    {

                        if (input = "e") then
                        {
                        	if(not (isvoid stack.top())) then
                        	{
                        		eval();
                        	}
                        	else
                        	{
                        		"";
                        	}
                        	fi;
                            
                        }
                        else
                        {
                            stack.push(input);
                        } fi;
                    } fi;
                    
                } pool;
            };
	}};

	  eval() : Object
    {{
       	let top: String,
       	    int1: Int,
       	    int2: Int,
            element1: String,
            element2: String
        in 
        {
            top <- stack.top().get_data();
            if (top = "+") then
            {
            	stack.pop();
                int1 <- a2i(stack.pop());
                int2 <- a2i(stack.pop());
                stack.push(i2a(int1 + int2));
            }
            else 
            {
                if (top = "s") then
                {
                    stack.pop();
                    element1 <- stack.pop();
                    element2 <- stack.pop();
                    stack.push(element1);
                    stack.push(element2);
                }
                else
                {
                    "";
                } fi;
            } fi;
        };
    }};
};
