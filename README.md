# Build, tests, and coverage

Build the program with GCC and Make:

```sh
make program
```

Run the automated backend unit tests with:

```sh
make test_backend
```

The tests use CUnit. `make test` runs an interactive listing scenario and expects
input from a terminal; it is not the automated unit-test target.

Line and branch coverage was measured with GCC's `gcov` 13.3.0, compiling with
`--coverage -O0`. The measurement combines the scripted front-end scenario from
`webstoretest.txt` with the backend CUnit suite (10 tests, 95 assertions).
Branch coverage below means the percentage of branch outcomes taken at least
once, as reported by gcov's `Taken at least once` metric.

| Source file | Line coverage | Branch coverage |
|---|---:|---:|
| `front_end.c` | 85.63% | 65.15% |
| `back_end.c` | 86.74% | 75.47% |
| `hash_table.c` | 65.88% | 50.00% |
| `hash_table_iterator.c` | 100.00% | 90.00% |
| `linked_list.c` | 81.11% | 58.33% |
| `linked_list_iterator.c` | 81.48% | 50.00% |
| `utils.c` | 59.82% | 38.89% |
| `backend_cunit_tests.c` | 98.53% | 55.00% |

# Using the program

After building, start the interactive program with:

```sh
./program
```

The program displays a menu for merchandise, inventory, and shopping-cart
operations. No input or data files are required: each run starts with an empty
in-memory database, which is discarded when the program exits. `webstoretest.txt`
is an optional scripted input used by the `make text` target; that target also
requires Valgrind.

# End-of-work review

`TODO.md` also lists writing header files and tests. Function specifications
have since been added to the headers and a backend CUnit suite exists; verify
these items are complete before closing them. Refactoring repeated code,
checking `size_t` versus `int` usage, and preventing zero-quantity additions to
a cart remain to be reviewed or resolved before the backlog is considered clear.

Backend string ownership is operation-dependent; callers and tests should account
for the current behavior:

- A successful `ioopm_create_merchandise` stores the supplied name and
  description pointers. If creation fails because the name already exists, the
  caller remains responsible for those strings.
- A successful `edit_merchandise` takes the replacement description and, when
  renaming, the replacement name. When the new name equals the existing name,
  the backend frees the supplied replacement-name string itself. On failure,
  the caller remains responsible for the replacement strings.
- `replenish` stores the supplied shelf string when creating a shelf. If that
  shelf identifier already exists, it frees the supplied string; if the
  merchandise name is not found, the caller remains responsible for it.

# Lab demonstration preparation

Demonstrate Z92, then use these three related feature areas to show inventory
moving through the store and into a completed purchase. Match each area to its
achievement ID in the course achievement list before presenting:

1. **Shelf inventory:** replenish merchandise on multiple shelves and show that
   replenishment updates stock. Evidence: `replenish` and
   `test_replenish_shelves` / `test_checkout_multiple_shelves`.
2. **Cart reservation and checkout:** add merchandise to a cart, show the
   calculated total, and check out; explain that stock is deducted and the
   reservation is released.
   Evidence: `add_to_cart`, `calculate_cost`, `checkout`, and the cart/checkout
   CUnit tests.
3. **Merchandise lifecycle:** list merchandise, edit an item's details, and
   remove an item. Evidence: `list_merchandise`, `edit_merchandise`,
   `remove_merchandise`, and their backend tests.

Share the demonstration between group members: have each person explain and
answer questions about the part they implemented or tested.

# Submission checklist

- [ ] Review and resolve or document the remaining backlog and any special
      implementation agreements.
- [ ] Email `ioopm@it.uu.se` with group members' names and usernames and a link to
      the GitHub repository.
- [ ] Make and push the final assignment commit, then tag it
      `assignment2_done`.
- [ ] Optionally provide feedback on the assignment.
