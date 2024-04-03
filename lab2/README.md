# 实验二：虚拟页式存储管理系统

【实验目的】

1. 理解虚拟内存和物理内存之间的关系及转化，在什么情况下会发生缺页中断。
2. 理解和掌握各种页面置换算法的工作原理，包括先进先出（FIFO）、最近最少使用（LRU）、时钟置换（CLOCK）和随机置换算法，并通过模拟实验观察它们的执行过程。
3. 通过比较不同页面置换算法下系统的性能，理解不同算法的优缺点以及在何种情况下更适用。
4. 学习如何评估系统性能。通过实验结果，理解如何通过缺页中断次数、平均访问时间等指标去评估系统性能，并能合理地解释实验结果。
5. 从实验中体验和理解内存访问的局部性原理，提高自己对操作系统虚拟内存管理的理解。

【实验要求】

1. 设计并实现一个模拟的简单虚拟页式存储管理系统。在该系统中，应能模拟以下功能：创建进程的页表，为进程页面分配内存块，实现逻辑地址向物理地址的转换。当发生缺页时，通过缺页中断处理调入页面，如果需要，可以进行页面置换。
2. 制定一套模拟数据，包括要访问的页面、需执行的操作等，并按照虚拟页式存储管理的原则进行处理，展示每一步的执行结果。
3. 能够模拟出页面置换的情况，例如FIFO、LRU、CLOCK和随机置换等算法，比较并分析每种算法下系统的性能。

【涉及的基本原理】

1. 虚拟内存技术：这是一种使得计算机系统看上去具有比实际RAM更大的内存的技巧。它使用了硬盘空间作为对RAM的扩充并将之作为应用程序的虚拟存储器。
2. 页（Page）：操作系统管理内存的一个单位。操作系统会把逻辑地址空间分割为一系列连续的、固定大小的区域，这些区域就称为页。
3. 帧（Frame）：操作系统管理对应实际内存的一个单位，与页的大小一致。
4. 页表：虚拟页面和物理内存帧之间的映射存放在页表中。为了提高地址转换的效率，操作系统通常会采取一些策略来优化页表，如多级页表、倒置页表、TLB（Translation Lookaside Buffer）等。
5. 地址映射：操作系统会维护一个页表，记录页号和帧号的映射关系。当CPU发出一个虚拟地址时，操作系统会通过页表将虚拟地址转换为实际的物理地址。
6. 页面置换：由于内存有限，当分配给一个进程的所有页面都在使用中且需要加载新的页面时，操作系统必须选择一个页面将其移出内存以空出空间给新的页面，这个决定过程就是页面置换。常见的页面置换算法包括最近最久未使用（LRU）、最佳置换算法（OPT）、先进先出（FIFO）、时钟置换（CLOCK）等。
7. 缺页中断：当程序引用的页不在内存中时，会产生缺页中断，操作系统需要从磁盘中将页调入内存，并更新页表。

【实验提示】

1. 设计进程类，包含该进程的页表，页表中应包含页号和对应的物理块号等。
2. 设计内存类，模拟内存中的物理块，能够根据进程请求分配和回收物理块。
3. 设计地址转换函数，输入逻辑地址，输出物理地址。
4. 实现页面置换算法，对内存中的页面进行动态调整。
5. 缺页中断通常是因为进程尝试访问其虚拟内存空间中一个不存在于物理内存（RAM）中的页面时触发的，如果分配给该进程的物理内存已满，则需要进行页面置换。
6. 为了展示该内存管理系统，需设计合理的模拟数据，当运行内存管理程序时，展示每一步的内存分配、地址转换和页面置换情况。
7. 在模拟不同页面置换算法时，请选择合适的测试用例来体现内存访问的局部性原理，比如循环和递归时会访问重复的页面集，而访问数组或其它大型数据结构时会访问一大块连续的内存页面，这些情况在测试时需要考虑。
8. 分析并比较不同页面置换算法下系统的性能，需要定义一些性能指标，比如缺页中断次数、平均访问时间等。

【验收标准】

1. 基本功能的实现：能正确执行虚拟地址向物理地址的转换，正确实现了四种页面置换算法，包括LRU（最近最久未使用）、FIFO（先入先出）、CLOCK（时钟算法）和随机置换算法，能够在内存空间不足时，正确选择被替换的页面。
2. 选择合理的测试数据进行展示：能够设计合理的模拟数据，展示虚拟内存管理过程，能够通过模拟数据触发缺页置换过程，而不是直接指定并执行页面置换程序。
3. 系统的稳定性和错误处理：程序在运行过程中是否能稳定执行，是否能有效地处理错误的情况（如页面错误、地址越界错误）。
4. 用户友好度：系统应当对用户友好，易于使用。具体表现在接口设计的合理性，输出信息的清晰性等。
5. 代码质量：高质量的代码表现在清晰的组织结构，良好的命名规范，适当的注释等。良好的代码质量可以提高代码的可读性和可维护性。
6. 实验报告：一个好的实验报告应该详细描述系统的设计、实现的功能及使用方法，测试情况，以及遇到的问题、解决方式等。在实验报告中需要包含采用不同页面置换算法时系统性能的分析比较，需要采用合理的性能指标。