#! /bin/bash
rm ./calico_log.txt
rm ./valgrind_log.txt

rm ../src/main

calico test.yaml |& tee calico_log.txt

valgrind --tool=memcheck --leak-check=full --show-leak-kinds=all ../src/main 1 5 ../data/map1.txt |& tee -a valgrind_log.txt
valgrind --tool=memcheck --leak-check=full --show-leak-kinds=all ../src/main 1 3 ../data/map2.txt |& tee -a valgrind_log.txt
valgrind --tool=memcheck --leak-check=full --show-leak-kinds=all ../src/main 1 4../data/map3.txt |& tee -a valgrind_log.txt
valgrind --tool=memcheck --leak-check=full --show-leak-kinds=all ../src/main 1 5 ../data/map4.txt |& tee -a valgrind_log.txt
valgrind --tool=memcheck --leak-check=full --show-leak-kinds=all ../src/main 1 1 ../data/map5.txt |& tee -a valgrind_log.txt
valgrind --tool=memcheck --leak-check=full --show-leak-kinds=all ../src/main 0 5../data/map1.txt |& tee -a valgrind_log.txt
valgrind --tool=memcheck --leak-check=full --show-leak-kinds=all ../src/main 0 3../data/map2.txt |& tee -a valgrind_log.txt
valgrind --tool=memcheck --leak-check=full --show-leak-kinds=all ../src/main 0 4../data/map3.txt |& tee -a valgrind_log.txt
valgrind --tool=memcheck --leak-check=full --show-leak-kinds=all ../src/main 0 5../data/map4.txt |& tee -a valgrind_log.txt
valgrind --tool=memcheck --leak-check=full --show-leak-kinds=all ../src/main 0 1../data/map5.txt |& tee -a valgrind_log.txt
