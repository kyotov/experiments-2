# Coding Guidelines

The following coding guidelines are followed for this repo:
1. Follow the guidelines and style outlined in the [Google C++ Style Guide](https://google.github.io/styleguide/cppguide.html).
2. Follow the guidelines in ["Effective Modern C++"](https://smile.amazon.com/Effective-Modern-Specific-Ways-Improve-ebook-dp-B00PGCMGDQ/dp/B00PGCMGDQ/ref=mt_other?_encoding=UTF8&me=&qid=1628969202) by Scott Meyers as long as they are not in conflict with the above rules.
3. Follow the guidelines specified below as long as they are not in conflict with the above rules.
4. Clang tidy checks have been enabled with a few exceptions (settings in .clang-tidy). These must succeed before landing a pull request on main.


## Guidelines to be followed (when not in conflict with the first two set of guidelines mentioned above)

Suggestions are (always) welcome.

1.	While we absolutely need forms of abstraction when implementing complex logic, recognize that not all abstraction is good. It can both increase and decrease complexity. Use abstraction if (and only if) the abstraction reduces overall complexity.

2.	C++ has a lot of exciting features. Use of features just because they exist can be counterproductive. Generally, if other mainstream languages (such as C or Java) don’t provide the type of feature you plan to use, recognize the extra complexity cost before deciding to use these. As an example, avoid using default parameters.

3.	There are at least three core users of the code you write:
    
    1. The end user who uses the library or the overall application that you produce
    
    2. Other developers who work on the code with you
    
    3. The machine that runs your code

All three are important and should be given consideration. On the boundary, prioritize the users in the above order. Note also that this says on the boundary. Putting in a lot of effort on (ii) in favor of (i) is generally not recommended.

4.	When implementing, solve for known and anticipated problems. It’s valuable to sometimes create reasonable expectations of additional anticipated problems. However, just like it’s good to avoid pre-mature optimization, it’s also good to avoid pre-mature functionality build. If you’re trying to decide whether to add complexity when handling current and anticipated use cases in favor or potential future needs, seriously consider whether this is a case of pre-mature functionality build.

5.	The data members in a class usually carry state and the methods usually perform actions.  For example, use this to make choices such as where to keep state information – prefer a static member in a class instead of a static member in a method unless you have a strong reason to do otherwise.

6.	Some specific modern C++ features for which we have specific recommendations as they are not already covered in the Google Style Guide:

    1.	Prefer using std::thread or std::async() when distributing work across multiple threads (over using something like pthread_create).

    2.	If exposing a library, consider using pimpl to better encapsulate the publicly exposed interface if the performance implications are not significant. Avoid using pimpl in internal code unless there are material implications to expected build times.

7.	Prefer to use signed variables unless you absolutely need unsigned variables.
