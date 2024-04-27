# ft_vox
C++ OpenGL voxel with advanced procedural generation

![Montains](assets/montain.png)
![Desert](assets/desert.png)




We generate a world using Perlin noise in the Classes/World/ChunkGenerator class.
The player can move their camera, flying in the air indefinitely throughout the world.
The Perlin noises that define the generation loop around every few thousand blocks, but not at the same pace,
making it almost an infinite world.
There is a simple gravity, block collision, and entity management system in place.

The abstract class Chunk allows us to use different types of child classes, enabling us to manage the data as needed.
The main child class we utilize is RLEChunk,
where we manage the blocks of the chunk as ribbons (rubans).

We experimented with the geometry shader.
In the CPU, we generate a buffer containing our face information.
Each face info includes the position of the block, its type, and the 'size of the face.'

'Size of the face' refers to grouping multiple faces of the same wall and block type,
reducing the number of triangles to display.

Inside the GPU, we create our triangles. However, this does not significantly improve the FPS

