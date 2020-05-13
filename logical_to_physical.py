def handle_file(filename):
    physical_memory = {}
    logical_addresses = []
    with open(filename, "r") as f:
        m, q, pml4_address = map(int, f.readline().split())
        for _ in range(m):
            paddr, value = map(int, f.readline().split())
            physical_memory[paddr] = value
        for _ in range(q):
            logical_addresses.append(int(f.readline()))
    return physical_memory, logical_addresses, pml4_address


def get_bits(address, start_bit, end_bit):
    num_from_ones = 2 ** (end_bit - start_bit + 1) - 1
    bits = address >> start_bit & num_from_ones
    return bits


def get_next_table_address(index, start_address, physical_memory):
    ph_addr = start_address + index * 8
    if (ph_addr not in physical_memory) or (not physical_memory[ph_addr] & 1):
        return -1
    else:
        temp_addr = get_bits(physical_memory[ph_addr], 12, 51)
        return temp_addr * (2 ** 12)


print(get_bits(15359, 0, 11))
filename = 'dataset_44327_15.txt'
physical_memory, logical_addresses, pml4_address = handle_file(filename)
with open('answer.txt', 'w') as f:
    for i in range(len(logical_addresses)):
        index = get_bits(logical_addresses[i], 39, 47)
        pdp_addr = get_next_table_address(index, pml4_address, physical_memory)
        if pdp_addr == -1:
            f.write('fault\n')
            continue
        index = get_bits(logical_addresses[i], 30, 38)
        pd_addr = get_next_table_address(index, pdp_addr, physical_memory)
        if pd_addr == -1:
            f.write('fault\n')
            continue
        index = get_bits(logical_addresses[i], 21, 29)
        pt_addr = get_next_table_address(index, pd_addr, physical_memory)
        if pt_addr == -1:
            f.write('fault\n')
            continue
        index = get_bits(logical_addresses[i], 12, 20)
        page_addr = get_next_table_address(index, pt_addr, physical_memory)
        if page_addr == -1:
            f.write('fault\n')
            continue
        offset = get_bits(logical_addresses[i], 0, 11)
        final = page_addr + offset
        f.write(str(final) + '\n')
