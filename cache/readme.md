Sweet Cache What Light Through Yonder Window Breaks?

# Cache
- L1 is small but fast
- - Per core
- - Many x86/x64 systems have indipendent data and instruction L1's.
- L2 is mid size mid speed
- L3 is large but slow
- - Some lower end x86/x64 systems won't have an L3

## Cache lines
- The CPU requests data from RAM in blocks of 64 bytes (32/128 also possible)
It doesn't store individual byte in cache but instead these in these cache lines. It does this to reduce bus traffic
- The CPU assumes we might need adjecent data and so reads/writes all 64 surrounding bytes. So when we need the next byte it has already been cached.
*NOTE* It doesn't read single bytes from RAM or store single bytes in cache.

- To work out which line an address comes from, divide the address by 64 and discard the remainder.
- - Any bytes which, when address is divided by 64, lead to the same result, share the same line in cache.

- The bytes in a cache line allow for reading in either direction without performance penalty.
So it doesn't matter if you step forward or backwards through an array.

*NOTE* Work on data in a way that compliments the way it is stored.. in consecutive bytes.

### RAM
- SRAM is fast but expensive
- DRAM is slow but cheap
Cache is made up of SRAM, System-Memory of DRAM.

x64 Memory Speed Comparison
*NOTE* The CPU does not always have to wait the clock cycles.

| Memory Type | Latency in Clock Cycles |
|:------------|------------------------:|
| Register    | 1                       |
| L1 Cache    | 3                       |
| L2 Cache    | 15                      |
| L3 Cache    | 60                      |
| Main Memory | 150                     |
| Hard Drive  | 1000000's               |

If we don't find what we need in one we must move to the next. Incurring a performance hit.

### Terms
- Cache Hit: is when the data we request is in cache
- Cache Miss: is when the data we request is not in cache and must be read from main memory
- Data Cache: is for storing arithmetic and computation data
- Instruction Cache: is for storing instructions that the CPU will perform
- Unified Cache: Can store both data and instructions
- Clock Cycles: One tick of the CPU's timer. Smalles possible duration for the device
- - For example: A 1GHz clock ticks 1 billion times per second.
- - Latency: Time it takes for a CPU to wait or perform some action is often measured in clock cycles.

## Matrix Product Example

- Naive approach 
To compute an element of the matrix product we need to read a row and a column from the input matrices A and B then compute the dot product.

[+] Product computed in 12.4593s

If our matrices are stored as rows in RAM then it is slow to read columns and vice versa as we are not using a majority of the cache line.

- Better approach (being considerate of cache lines)
If we store our matrices in rows and columns, we can read the row version of data from matrix A, and the column data from matrix B.
To store as column, swap X and Y co ordinates.

[+] Product computed in 9.35752s

Although utilizing twice the RAM (and running at half speed where having to perfom double the operations), by better working with our cache, and requiring less reads, we can gain a significant reduction in latency, since now whether reading rows or columns we are reading the whole cache line.

### Cache Blocking

Blocking is the technique of splitting a large problem into blocks to reduce the working set of data.
By working on small blocks that fit into cache we can reduce cache pollution and improve performance.

### Cache Thrashing and Cache pollution

Normally when computing the matrix product we effectively step through the first row of the first matrix, and every column of the second matrix.
Having read the first row we want the next, but in cache we will have something like the last column. 
When the cache is filled with data we don't need, it is considered cache pollution.

Thrashing is the act of filling the cache with data and not using it. For example stepping through a large array and filling the cache with data that is not needed after or that will be evicted before being being recalled again.


### Blocking The Matrix Example
...




Sources resources and further explanations:

- Performance x64: Caches 1 - https://youtu.be/bHzrhH7yySA

- Recitation 5 Cache Lab and Blocking - https://www.youtube.com/watch?v=HFa1NkFTeSw

- CppCon 2017: Carl Cook “When a Microsecond Is an Eternity: High Performance Trading Systems in C++” - https://www.youtube.com/watch?v=NH1Tta7purM
