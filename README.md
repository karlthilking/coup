coup
====
coup is an easy-to-use, multithreaded, and descriptive build tool for managing c++ projects.
It handles the build process from compiling source code, cleaning object files and executables,
and running executables, all through simple commands and minimal JSON configuration files.

Goals
-----
* Provide easy-to-use and out-the-box functionality.
* No DSL for setting up projects, simple JSON, a familiar and readable format.
* Efficiency: compilation steps should be very quick by utilizing the avaiable hardware.
* Informative logging to provide clear insight into every step during the build process.
* Flexibility: coup should be able to service any c++ project's demands

Note
----
My idea of what a complete version of coup should look like is not yet completely
well-defined, thus, features, commands, configuration settings, etc. are subject
to change. Additionally, the goals and scope of this project are subject to change
as well. However, there are a few immediate plans I have to extend current functionality.
Continue reading for more information.

Short-Term Plans
----------------
* Add support for external libraries, as well as unit test frameworks and automating testing.
* Implement avoiding redundant work (i.e. unnecessary recompilation)
* Optimize the fast-path, where significant information is provided and compilation is straighforward
* Allow for a slow-path, information is missing, but tool can default to searching filetree and being smart
* Utilize caching to resolve over-extensive information retrieval for each repeated step 

Long-Term Plans
---------------
* Shall coup reach a mature, robust state, develop sufficient documentation for using coup
