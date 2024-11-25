Frogger Game based C++ ncurses

# Compiling

## Debug 

```
cmake -DCMAKE_BUILD_TYPE=Debug -G "Unix Makefiles" -S "<ROOT PROJECT>" -B "<ROOT PROJECT>/debug"
```

```
cmake --build "<ROOT PROJECT>/debug" --target frogger -- -j <N-CORE COMPILE>
```

## Release

```
cmake -DCMAKE_BUILD_TYPE=Debug -G "Unix Makefiles" -S "<ROOT PROJECT>" -B "<ROOT PROJECT>/release"
```

```
cmake --build "<ROOT PROJECT>/release" --target frogger -- -j -j <N-CORE COMPILE>
```
