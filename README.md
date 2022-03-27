<p align="center"> <img src="icon.svg" width="200"></img></p>
EBS is a really fast build system that helps you make your build scenarios more shorter and readable. Examples can be found at examples/ folder.

Example of build scenario:

```ruby
component static libOmegaThread: 
	"src/omega/omega_thread.c"
	"src/omega/omega_WinThreads.c";

component static libOmega: "src/omega/omega.c" 
	"src/omega/omega_renderer.c"
	"src/omega/omega_Win32.c"
	"src/omega/omega_Linux.c"
	"src/omega/omega_OSX.c";

link libOmega libOmegaThread;

component executable omegaRendererTest: "src/omega/tests/omega_renderer_test.c";
link omegaRendererTest libOmega;
test omegaRendererTest;
```

## Building
Linux platform is really recommended. For building EBS, you need to have [gnu c compiler](https://gcc.gnu.org/releases.html). Then use build bash script in `src/` folder (`src/build.sh`).

## Syntax

### Components
Components are the main part of EBS. Components are group of files that can be compiled together. There are two types of components: static and executable. Static components are compiled into a static object file, and executable components are compiled into an executable. To declare a component you need to use `component` keyword and the syntax as follows:
```
component <static/executable> name: "source1.c" 
  "source2.c" 
  ...;
```

You can also link components together. To link components together you need to use `link` keyword and the syntax as follows:

```
link mainComponent
  linkComponent1
  linkComponent2 ...;
```

### Tests
EBS also supports testing. All executable components can be tested, which means that EBS checks for an exit code of the executable, because most of testing frameworks for C especially use assertions, to cause the program to exit with an error code. To test a component you need to use `test` keyword and the syntax as follows:

```
test componentName;
```

That's all! As you can see EBS is really simple and easy to use, so it can be used for small projects, which don't require a lot of additional features, that CMake is suitable for.