#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <fstream>
#include <streambuf>
#include <iostream>

#include <dukglue.h>

/* A test object for doing pass by reference with duktape and accessing member functions from scripts */
class Vector2f
{
public:
	Vector2f(double x, double y) : m_fX(x), m_fY(y) {}
	Vector2f() { m_fX = 0; m_fY = 0; }

	float Dot(Vector2f& other)
	{
		return m_fX * other.GetX() + m_fY * other.GetY();
	}

	float Distance(Vector2f& other)
	{
		return sqrt(pow(m_fX - other.GetX(), 2) + pow(m_fY - other.GetY(), 2));
	}

	/* Getters and setters */
	double GetX() { return m_fX; }
	double GetY() { return m_fY; }
	void SetX(float x) { m_fX = x; }
	void SetY(float y) { m_fY = y; }

private:
	double m_fX;
	double m_fY;
};

static void load_script_from_file(duk_context* ctx, const char* filename) // Note: I think it's better to take a const char* as an input because this is what string literals are. 
{																		  //		If this took a string, the compiler would add additional steps to convert the string literal to a string object.
	std::ifstream t(filename);
	std::stringstream buffer;
	buffer << t.rdbuf();
	duk_push_lstring(ctx, buffer.str().c_str(), (duk_size_t)(buffer.str().length()));
}

static duk_ret_t native_print(duk_context* ctx)
{
	duk_push_string(ctx, " ");
	duk_insert(ctx, 0);
	duk_join(ctx, duk_get_top(ctx) /* #args -> need to do this if we pass DUK_VARARGS when pushing this c_function */
		- 1);
	printf("%s\n", duk_safe_to_string(ctx, -1));
	return 0;
}

int main()
{
	// Create a heap and initial context
	duk_context* ctx = NULL;

	ctx = duk_create_heap_default();
	if (!ctx) {
		printf("Failed to create a Duktape heap.\n");
		exit(1);
	}

	// Register objects and member functions inside our context
	dukglue_register_constructor<Vector2f>(ctx, "Vector2f"); // Can this be done more dynamically? i.e. to allow client to register objects to work with scripts?
	dukglue_register_method(ctx, &Vector2f::Dot, "dot");
	dukglue_register_method(ctx, &Vector2f::Distance, "distance");
	dukglue_register_method(ctx, &Vector2f::GetX, "x");
	dukglue_register_method(ctx, &Vector2f::GetY, "y");

	// Can use the standard duktape API to register c_functions if necessary
	duk_push_c_function(ctx, native_print, DUK_VARARGS);
	duk_put_global_string(ctx, "print");

	// Load script from file, evaluate script
	load_script_from_file(ctx, "scripts/testScript.js"); // This helper function pushes the script as a string to the value stack
	if (duk_peval(ctx) != 0) {
		printf("Error: %s\n", duk_safe_to_string(ctx, -1));
		duk_destroy_heap(ctx);
		return 1;
	}
	duk_pop(ctx); // Ignore return, clear stack

	duk_push_global_object(ctx);			// [...] -> [... global] (top)
	duk_get_prop_string(ctx, -1, "myTest"); // Accesses the object at index -1, i.e. the element at the top of the stack, the global object.
											//	Gets the VALUE associated with the KEY string that belongs to said object, puts it at the top of the stack.
											// i.e. gets the function associated with the string identifier myTest, places the function at the top of the stack 
											// [... global] ->[... global myTest]

	/* This demonstrates passing objects declared in C++ to scripts */
	Vector2f a(2, 3); Vector2f b(4, 5);		
	dukglue_push(ctx, &a);
	dukglue_push(ctx, &b);					// [... global myTest] -> [... global myTest a b]

	if (duk_pcall(ctx, /* Number of arguments to pull of the stack for the function call */ 2 ) != 0) // duk_pcall calls the function we specified on the stack (myTest) with number of arguments
		printf("Error: %s\n", duk_safe_to_string(ctx, -1));											  // that we specify, then places any return value at the top of the stack (index -1)
	else
		printf("%s\n", duk_safe_to_string(ctx, -1));

	duk_pop(ctx);

	duk_destroy_heap(ctx);

	return 0;
}