# render_project
realistic 3d renderer and scene editor in C

project can be built with make.

rendeng programm expects filepath to a sceene file.
sceene files defines what the rendeng programm should render.

scene files consist of materials, lights and objects.

material can be specified like this:

m [r] [g] [b] [shine] [reflectivnes] [refractivnes] [refractive_index]
on specification a material is assigned an index in the oder of specification to be used in object specifications


light is specified like this:

Markup : `code()`

l [x] [y] [z]  [r] [g] [b]    [r] [g] [b]
              diffuse color  specular color
