# render_project #
realistic 3d renderer and scene editor in C. 
project can be built with make. 
rendeng programm expects filepath to a sceene file. 
sceene files defines what the rendeng programm should render. 
scene files consist of materials, lights and objects. 

### material can be specified like this: ###
```
m [r] [g] [b] [shine (int)] [reflectivnes] [refractivnes] [refractive_index]
```
on specification a material is assigned an index in the oder of specification (starging with 0) to be used in object specifications

### light is specified like this: ###
```
l [x] [y] [z]  [r] [g] [b]    [r] [g] [b]
              diffuse color  specular color 
```

### plane is pecified like this: ###
```
p [x] [y] [z]   [x] [y] [z]   [material_index (int)]
   position    surface normal
*surface normal specifies only the direction. does not need to be normalized
```

### sphere is pecified like this: ###
```
s [x] [y] [z] [r] [material_index (int)]
    center
```

all values are floats if not specified other wise.
example sceene file: test_sceene
