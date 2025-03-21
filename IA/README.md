# ROTEIRO CONTROLADOR COM IA 

1) Elaborar modelo de idenficação de sistemas Y(x) que retorna a saída a partir das n últimas entradas anteriores (modelo de machine learning)

2) Simular o sistema com novos valores e comparar com os dados originais

3) Estabelecer uma função do controlador $y[n] = a_1\cdot x[n-1] + a_2\cdot x[n-2] + a_3\cdot x[n-3] + ... + b_1\cdot y[n-1] + b_2\cdot y[n-2] + b_3\cdot y[n-3] + ...$ (onde $x$ é setpoint - input = erro, y é a saída do controlador)

5) Criar uma resposta almejada com os valores de Ts, UP, finalValue que chegam a um setpoint a partir de uma entrada e uma condição inicial Yo (obtido a partir do modelo de identificação de sistemas). A entrada da simulação é a saída do controlador $y[n]$

4) Simular o sistema com a mesma entrada datafitting que irá selecionar os melhores coeficientes de  $y[n]$ que mais se aproximam dessa resposta.
