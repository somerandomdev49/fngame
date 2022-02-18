// MacOS's built in libc++ doesn't support std::filesystem until MacOS 10.15.
// This is a wrapper around the default filesystem header that provides the necessary
// classes and methods if using MacOS <10.15

// #if 
