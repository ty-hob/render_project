file = open("untitled.obj", "r")

vertecies = []
faces = []

for l in file:
    if l[0:2] == "v ":
        l = list(l)
        del l[0:2]
        del l[-1]
        l = "".join(l)
        l = l.split(" ")
        l = [float(i) for i in l]
        vertecies.append(l)
    elif l[0:2] == "f ":
        l = list(l)
        del l[0:2]
        del l[-1]
        l = "".join(l)
        l = l.split(" ")
        face = []
        for v in l:
            v = v.split("/")
            face.append(int(v[0]) - 1)
        faces.append(face)


scaller = 70
adder = 0

adder = [500, -50, 0]

for f in faces:
    obj_string = "add_triangle(&objm, "
    for v in f:
        for i in range(len(vertecies[v])):
            obj_string += str((vertecies[v][i] * scaller) + adder[i]) + ", "
        # for p in vertecies[v]:
        # 	obj_string += str((p+adder)*scaller) + ', '
    obj_string += "&mat_trig1, 0);"

    print(obj_string)

# for v in vertecies:
# 	print(str(v))
