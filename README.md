# experiments
Small experiments for new product and code style trials.

These are expected to serve as a means to try different prototypes and enable comparisons across ideas and languages. This repo partially originated to test a null hypothesis of the following form:
"For a trading systems software developer who has spent at least 2 months on understanding C++ language features, the productivity difference in using Java over C++ is relatively small if any (in this hypothesis, less than 25% extra time needed for a C++ implementation relative to the time for a Java implementation). This incorporates equivalent runtime performance and equivalent readability and maintainability."

There are obviously a lot of subjective aspects in the above statement, and the trials only scratch the surface in identifying potential examples that may reject the above hypothesis. So far, the trials have included:
1. Implementation of an iterator for a non-terminating prime numbers sequence. (iterators/)
2. Implementation of an expressions evaluator and simplifier. E.g. it evaulates 5\*3-3 and returns 12 and it simplifies a\*(b+c) to a\*b+a\*c. (expressions/)

Some additional examples were also tried in an attempt to identify whether C++ adds material complexity or safety risk over Rust in certain use cases. Related trials include:
1. Specific examples of potential C++ errors and validation on whether clang tidy checks catch these. (code_checks/)
2. Simple example of using templates to enable writes to different structures with common elements. (type_size_optimizations/)
3. Simple example of using async to run multiple threads in parallel, all acting on and reading from the same input state. (multi_reader_async/)


Acknowledgements: Thanks to Kamen for discussions and ideas on potential usage patterns, suggestions for different experiments, and recommendations on specific language features.
