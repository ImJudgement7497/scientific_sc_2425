#!/bin/bash

make clean
make simple_md_C_pg
./simple_md_C_pg
diff simple_md.log ./results/simple_md.log.ref
gprof ./simple_md_C_pg >> ./results/viking_profile.txt
make clean
