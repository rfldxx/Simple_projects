template<size_t N, size_t M>
class Microsheme {
    ??? field[N][M];
    
}


// ----------------------------
auto  ORchip = Microsheme<3, 4>(...);
auto ANDchip = Microsheme<3, 4>(...);

// Люто образно:
auto  my_pc  = Microsheme<2, 1>(ORchip + ANDchip);

// подали сигнал редстоуна силы 14 во второй порт микросхемы:
my_pc[2] = 14;
my_pc[1] =  0;

// запустили моделирование процесса
my_pc.launch();

// вывод того, какой сигнал в итоге на портах:
for(p : my_pc.ports) cout << p;
