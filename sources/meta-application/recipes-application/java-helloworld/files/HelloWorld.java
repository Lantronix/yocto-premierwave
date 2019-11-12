/* This is a simple Java program. 
FileName : "HelloWorld.java". */
class HelloWorld 
{ 
    // Your program begins with a call to main(). 
    // Prints "Hello, World" to the terminal window. 
    public static void main(String args[]) 
    { 
        while(true)
        {
            System.out.println("Hello, World"); 
            String version = System.getProperty("java.version");
            System.out.println(version);
            try{
                Thread.sleep(10000);
            }catch (Exception e) {}
        }
    } 
} 

