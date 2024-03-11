#pragma once
namespace Math {
	constexpr float TAU{ 6.2831f }; //tau is pi times 2


	template<typename Type> //define function below this as template. Mostly used in header files
	Type Max(Type a, Type b) { //this is now a templated function. Like Macros, these are copy-pasted with the types that are given.

		//ternary operator (short if-else statement)
		return a > b ? a : b; //if a>b true --> do a, else do b.
	};

	template<typename Type>
	Type Lerp(Type a, Type b, float t) {
		return a + (b - a) * t;
	}

}

//example usage of template with different types. Whenever we do this, the compiler creates a new function with the new type. 
//int intMax = Math::Max<int>(5, 10);
//float floatMax = Math::Max<float>(5, 10);
//double doubleMax = Math::Max<double>(5, 10);

