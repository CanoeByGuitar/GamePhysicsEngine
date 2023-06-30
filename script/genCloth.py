def generate_rectangle_obj_file(m, n, width, height, filename):

    m_col = m
    m_row = n
    grid_num = m_col * m_row
    m_indicesSize = grid_num * 2 * 3
    m_bufferSize = (m_col + 1) * (m_row + 1) * 3

    vertices = [0.0] * m_bufferSize
    face = [0] * m_indicesSize

    for i in range(m_row + 1):
        for j in range(m_col + 1):
            # x, y, z of one vertex
            vertices[3 * (i * (m_col + 1) + j) + 0] = float(i) / float(m_row) * width
            vertices[3 * (i * (m_col + 1) + j) + 1] = float(j) / float(m_col) * height
            vertices[3 * (i * (m_col + 1) + j) + 2] = 0.0

    for i in range(m_row):
        for j in range(m_col):
            grid_index = i * m_col + j
            face[6 * grid_index + 0] = i * (m_col + 1) + j
            face[6 * grid_index + 1] = (i + 1) * (m_col + 1) + j
            face[6 * grid_index + 2] = (i + 1) * (m_col + 1) + j + 1
            face[6 * grid_index + 3] = i * (m_col + 1) + j
            face[6 * grid_index + 4] = (i + 1) * (m_col + 1) + j + 1
            face[6 * grid_index + 5] = i * (m_col + 1) + j + 1


    # Save to .obj file
    with open(filename, 'w') as obj_file:
        for i in range(0, len(vertices), 3):
            obj_file.write(f"v {vertices[i]} {vertices[i + 1]} {vertices[i + 2]}\n")

        for i in range(0, len(face), 3):
            obj_file.write(f"f {face[i] + 1} {face[i + 1] + 1} {face[i + 2] + 1}\n")

    print(f"Generated {filename} successfully.")


# 示例用法
generate_rectangle_obj_file(20, 20, 1, 1, 'cloth20x20(small).obj')