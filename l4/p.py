from collections import defaultdict

def compute_first(symbol):
    if symbol not in non_terminals:
        return set(symbol)

    if symbol in first and first[symbol]:
        return first[symbol]

    first[symbol] = set()

    for prod in grammar[symbol]:
        if prod == 'ε':
            first[symbol].add('ε')
        else:
            for sym in prod:
                sym_first = compute_first(sym)
                first[symbol].update(sym_first - set('ε'))
                if 'ε' not in sym_first:
                    break
            else:
                first[symbol].add('ε')

    return first[symbol]

def compute_follow(symbol):
    if symbol in follow and follow[symbol]:
        return follow[symbol]

    follow[symbol] = set()
    if symbol == start_symbol:
        follow[symbol].add('$')

    for head, prods in grammar.items():
        for prod in prods:
            for i in range(len(prod)):
                if prod[i] == symbol:
                    for j in range(i+1, len(prod)):
                        first_next = compute_first(prod[j])
                        follow[symbol].update(first_next - set('ε'))
                        if 'ε' in first_next:
                            continue
                        else:
                            break
                    else:
                        if head != symbol:
                            follow[symbol].update(compute_follow(head))

    return follow[symbol]

grammar = defaultdict(list)
non_terminals = set()

print("Enter the grammar productions (empty line to end):")
while True:
    line = input().strip()
    if not line:
        break
    head, bodies = line.split("->")
    head = head.strip()
    non_terminals.add(head)
    for body in bodies.split('|'):
        grammar[head].append(body.strip())

start_symbol = list(grammar.keys())[0]

first = {}
follow = {}

for nt in non_terminals:
    compute_first(nt)

for nt in non_terminals:
    compute_follow(nt)

print("\nFirst sets:")
for nt in non_terminals:
    print(f"First({nt}) = {{ {', '.join(first[nt])} }}")

print("\nFollow sets:")
for nt in non_terminals:
    print(f"Follow({nt}) = {{ {', '.join(follow[nt])} }}")
