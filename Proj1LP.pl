% Projeto realizado por Alexandre Ferreira ist1103397
%                    MBL
% alexandre.bruno.ferreira@tecnico.ulisboa.pt para mais informacoes
:-[codigo_comum].

%----------------------------------------------------------------------------
%    2.1 - extrai_ilhas_linha(N_L,Linha,Ilhas)
%
%    Devolve Ilhas, a lista ordenada das ilhas da linha
%
%----------------------------------------------------------------------------

extrai_ilhas_linha(N_L,Linha,Ilhas) :- extrai_ilhas_linha(N_L,Linha,Ilhas,[],0). % predicado engordado com um contador para obter a coluna de uma ilha
extrai_ilhas_linha(_,[],Aux,Aux,_).
% quando a cabeca contem um numero diferente de 0 ou seja uma ilha
extrai_ilhas_linha(N_L,[C|R],Ilhas,Aux,X):- NX is X + 1,
                                                    C > 0,
                                                    append(Aux,[ilha(C,(N_L,NX))],NovoAux),
                                                    extrai_ilhas_linha(N_L,R,Ilhas,NovoAux,NX).
% quando a cabeca e zero (ignorar)
extrai_ilhas_linha(N_L,[C|R],Ilhas,Aux,X) :-    NX is X + 1,
                                                    C = 0,
                                                    extrai_ilhas_linha(N_L,R,Ilhas,Aux,NX).
%----------------------------------------------------------------------------
%    2.2 - ilhas(N_L,Linha,Ilhas)
%
%    Devolve Ilhas, a lista ordenada das ilhas de um puzzle
%
%----------------------------------------------------------------------------

ilhas(Puz,Ilhas) :- ilhas(Puz,Ilhas,[],0).
ilhas([],Aux,Aux,_).
ilhas([C|R],Ilhas,Aux,X) :- NX is X + 1,
                          extrai_ilhas_linha(NX,C,IlhasLinha), % mesmo processo de extrai_ilhas_linha
                          not(IlhasLinha == []),               
                          append(Aux,IlhasLinha,NovoAux),
                          ilhas(R,Ilhas,NovoAux,NX).
ilhas([C|R],Ilhas,Aux,X) :- NX is X + 1,
                            extrai_ilhas_linha(NX,C,IlhasLinha),
                          IlhasLinha == [],
                          ilhas(R,Ilhas,Aux,NX).
%----------------------------------------------------------------------------
%    2.3 - vizinhas(Ilhas,Ilha,Vizinhas)
%
%    Devolve Vizinhas, a lista das ilhas vizinhas de Ilha
%
%----------------------------------------------------------------------------

vizinhas(Ilhas,ilha(P,(N_L,C)),Vizinhas) :- vizinhas(Ilhas,ilha(P,(N_L,C)),Vizinhas,[],0).
% parte de cima
vizinhas(Ilhas,ilha(P,(N_L,C)),Vizinhas,Aux,Fin) :- findall(ilha(P1,(L,C1)),(member(ilha(P1,(L,C1)),Ilhas),C1 = C),AuxPosV), % encontra ilhas acima de Ilha
											nth1(Y,AuxPosV,ilha(P,(N_L,C))),                                                % encontra indice para verificar existencia
                                            (findall(ilha(P1,(L,C1)),(member(ilha(P1,(L,C1)),Ilhas),C1 =:= C),AuxPosV),     % de vizinha superior
                                            nth1(Y,AuxPosV,ilha(P,(L,C))),IndSup is Y - 1,IndInf is Y + 1,IndSup >= 1,
                                            nth1(IndSup,AuxPosV,IlhaSup),append(Aux,[IlhaSup],NovoAux),
                                            vizinhas(Ilhas,ilha(P,(N_L,C)),Vizinhas,NovoAux,Fin,AuxPosV,IndInf));
                                            (findall(ilha(P1,(L,C1)),
                                            (member(ilha(P1,(L,C1)),Ilhas),C1 = C),AuxPosV),
                                            nth1(Y,AuxPosV,ilha(P,(L,C))),IndSup is Y - 1,IndInf is Y + 1,IndSup < 1,
                                            vizinhas(Ilhas,ilha(P,(N_L,C)),Vizinhas,Aux,Fin,AuxPosV,IndInf)).
% linha (esquerda e direita)                                      
vizinhas(Ilhas,ilha(P,(N_L,C)),Vizinhas,Aux,Fin,AuxPosV,IndInf) :- findall(ilha(P1,(L,C1)),
																(member(ilha(P1,(L,C1)),Ilhas),L =:= N_L),AuxPosH), % encontra ilhas na mesma linha 
														        nth1(Y,AuxPosH,ilha(P,(L,C))), % verificar indice de ilha nessa lista, para encontrar vizinhas 
													            vizinhas(Ilhas,ilha(P,(N_L,C)),Vizinhas,Aux,Fin,AuxPosV,IndInf,AuxPosH,Y).
vizinhas(Ilhas,ilha(P,(N_L,C)),Vizinhas,Aux,Fin,AuxPosV,IndInf,AuxPosH,Y) :- (IndAnt is Y - 1,IndSeg is Y + 1,IndAnt < 1,length(AuxPosH,X), IndSeg > X,
                                                 vizinhas(Ilhas,ilha(P,(N_L,C)),Vizinhas,Aux,Fin,AuxPosV,IndInf,_,_,0));% nenhuma vizinha
                                                 (IndAnt is Y - 1,IndSeg is Y + 1,
                                                 IndAnt >= 1,length(AuxPosH,X), IndSeg =< X,nth1(IndAnt,AuxPosH,IlhaAnt),
                                                 nth1(IndSeg,AuxPosH,IlhaSeg),append(Aux,[IlhaAnt,IlhaSeg],NovoAux),
                                                 vizinhas(Ilhas,ilha(P,(N_L,C)),Vizinhas,NovoAux,Fin,AuxPosV,IndInf,_,_,0)); % vizinha em ambos lados
                                                 (IndAnt is Y - 1,IndSeg is Y + 1,
                                                 IndAnt < 1,length(AuxPosH,X), IndSeg =< X, 
                                                 nth1(IndSeg,AuxPosH,IlhaSeg),append(Aux,[IlhaSeg],NovoAux) 
                                                 ,vizinhas(Ilhas,ilha(P,(N_L,C)),Vizinhas,NovoAux,Fin,AuxPosV,IndInf,_,_,0)); % viznha apenas a direita
                                                 (IndAnt is Y - 1,IndSeg is Y + 1,IndAnt >=1,length(AuxPosH,X), IndSeg > X,
                                                 nth1(IndAnt,AuxPosH,IlhaAnt),append(Aux,[IlhaAnt],NovoAux),
                                                 vizinhas(Ilhas,ilha(P,(N_L,C)),Vizinhas,NovoAux,Fin,AuxPosV,IndInf,_,_,0)).  % vizinha apenas a esquerda
	
             
% parte de baixo
vizinhas(Ilhas,ilha(P,(L,C)),Vizinhas,Aux,Fin,AuxPosV,IndInf,_,_,Pass) :- (Fim is Fin + 1,length(AuxPosV,X), IndInf =< X,nth1(IndInf,AuxPosV,IlhaInf),append(Aux,[IlhaInf],NovoAux), % mudanca do valor de Fin,
                                                                vizinhas(Ilhas,ilha(P,(L,C)),Vizinhas,NovoAux,Fim,AuxPosV,IndInf,_,_,Pass,0)); % tem vizinha em baixo                 para terminar o processo
                                                                (Fim is Fin + 1, length(AuxPosV,X), IndInf > X,vizinhas(Ilhas,ilha(P,(L,C)),Vizinhas,Aux,Fim,AuxPosV,IndInf,_,_,Pass,0)).
% caso base
vizinhas(_,_,Aux,Aux,1,_,_,_,_,_,_).

%----------------------------------------------------------------------------
%    2.4 - estado(Ilhas,Estado)
%
%    Devolve Estado, a lista ordenada cujos elementos sao entradas referente a cada uma
%    das ilhas de Ilhas, em que o 1o elemento e a ilha, o 2o as vizinhas dessa ilha e 3o as pontes  
%----------------------------------------------------------------------------

estado(Ilhas,Estado) :- estado(Ilhas,Estado,[],Ilhas).
estado([],Aux,Aux,_).
estado([C|R],Estado,Aux,Ilhas) :- vizinhas(Ilhas,C,A),append(Aux,[[C,A,[]]],NovoAux),
						estado(R,Estado,NovoAux,Ilhas).

%----------------------------------------------------------------------------
%    2.5 - posicoes_entre(Pos1,Pos2,Posicoes)
%
%    Devolve Posicoes, as posicoes entre a Pos1 e a Pos2
%----------------------------------------------------------------------------

posicoes_entre((X1,Y1),(X2,Y2),Pos) :- 	X1 = X2,(bagof(YV,between(Y1,Y2,YV),Ls1),bagof((X1,Y), % verificar qual das posicoes varia
														(member(Y,Ls1),Y =\= Y1,Y =\= Y2),Pos));
					                    Y1 = Y2,(bagof(XV,between(X1,X2,XV),Ls2),bagof((X,Y1), % utilizar bagof para obter a lista das posicoes entre 
                                                        (member(X,Ls2),X =\= X1,X =\= X2),Pos)); % pois caso nao existam posicoes entre, deve falhar
                                        X1 = X2,(bagof(YV,between(Y2,Y1,YV),Ls1),bagof((X1,Y),
                                                        (member(Y,Ls1),Y =\= Y1,Y =\= Y2),Pos));
                                        Y1 = Y2,(bagof(XV,between(X2,X1,XV),Ls2),bagof((X,Y1),
                                                        (member(X,Ls2),X =\= X1,X =\= X2),Pos)).

%----------------------------------------------------------------------------
%    2.6 - cria_ponte(Pos1,Pos2,Ponte)
%
%    Devolve Ponte, a ponte entre Pos1 e Pos2
%----------------------------------------------------------------------------

cria_ponte((X1,Y1),(X2,Y2),Ponte) :- sort([[X1,Y1],[X2,Y2]],L),cria_ponte((X1,Y1),(X2,Y2),Ponte,L). % sort utilizado caso as posicoes das pontes 
cria_ponte((X1,Y1),(X2,Y2),ponte((X1,Y1), (X2,Y2)),[[X1,Y1]|_]).                                   % nao sejam dadas por ordem
cria_ponte((X1,Y1),(X2,Y2),ponte((X2,Y2), (X1,Y1)),[[X2,Y2]|_]).                           

%----------------------------------------------------------------------------
%    2.7 - caminho_livre(Pos1,Pos2,Posicoes,I,Vz)
%
%    Devolve um true caso a adicao de uma ponte entre Pos1 e Pos2 nao faca com que
%    I e Vz deixem de ser vizinhas. Caso contrario, devolve false
%----------------------------------------------------------------------------

caminho_livre(Pos1,Pos2,Posicoes,ilha(_,(X1,Y1)),ilha(_,(X2,Y2))) :- posicoes_entre((X1,Y1),(X2,Y2),Entre), 
                                        findall(X,(member(X,Entre),member(X,Posicoes)),Res), % encontra as posicoes em da ponte e das posicoes entre as duas ilhas
                                        caminho_livre(Pos1,Pos2,Posicoes,ilha(_,(X1,Y1)),ilha(_,(X2,Y2)),Res).      
caminho_livre(Pos1,Pos2,Posicoes,ilha(_,(X1,Y1)),ilha(_,(X2,Y2)),Res) :- length(Posicoes,X),length(Res,Y),
                                                (X =:= Y,(Pos1=(X1,Y1),Pos2=(X2,Y2);Pos1=(X2,Y2),Pos2=(X1,Y1)) ; Y =:= 0).
                                                 % ou todas posicoes da ponte e entre ilhas sao comuns, ou nenhuma eh. so assim o caminho eh livre

%---------------------------------------------------------------------------- 
%    2.8 - actualiza_vizinhas_entrada(Pos1,Pos2,Posicoes,Entrada,Nova_Entrada)
%
%    Devolve Nova_Entrada, que eh igual a Entrada, so que com a sua lista de vizinhas actualizada
%    depois da introducao de uma ponte entre Pos 1 e Pos2
%----------------------------------------------------------------------------



actualiza_vizinhas_entrada(Pos1,Pos2,Posicoes,[ilha(P, (X1,Y1)),Viz,Pontes],Nova) :- 
             actualiza_vizinhas_entrada(Pos1,Pos2,Posicoes,[ilha(P, (X1,Y1)),Viz,Pontes],Nova,Viz,[]).
             
actualiza_vizinhas_entrada(_,_,_,[ilha(P, (X1,Y1)),_,Pontes],[ilha(P, (X1,Y1)),Aux,Pontes],[],Aux).
actualiza_vizinhas_entrada(Pos1,Pos2,Posicoes,[ilha(P, (X1,Y1)),Viz,Pontes],Nova,[C|R],Aux) :- 
            (caminho_livre(Pos1,Pos2,Posicoes,ilha(P, (X1,Y1)),C),  % verifica se cada uma das ilhas viznhas tem caminho livre 
            append(Aux,[C],NovoAux),                             % para a ilha, se tiver adiciona ao auxiliar da solucao
            actualiza_vizinhas_entrada(Pos1,Pos2,Posicoes,[ilha(P, (X1,Y1)),Viz,Pontes],Nova,R,NovoAux);
            actualiza_vizinhas_entrada(Pos1,Pos2,Posicoes,[ilha(P, (X1,Y1)),Viz,Pontes],Nova,R,Aux)).
            
          
%----------------------------------------------------------------------------
%    2.9 - actualiza_vizinhas_apos_pontes(Estado,Pos1,Pos2,Posicoes,Nova_Estado)
%
%    Devolve Nova_Estado, que eh igual ao Entrada, so que com a lista de vizinhas de cada ilha actualizada
%    depois da introducao de uma ponte entre Pos 1 e Pos2
%----------------------------------------------------------------------------

actualiza_vizinhas_apos_pontes(Estado,Pos1,Pos2,Novo_Estado) :-  posicoes_entre(Pos1,Pos2,Posicoes),
                         maplist(actualiza_vizinhas_entrada(Pos1,Pos2,Posicoes),Estado,Novo_Estado).

%----------------------------------------------------------------------------
%    2.10 - ilhas_terminadas(Estado,Ilhas_term)
%
%    Devolve IIlhas_term, a lista das ilhas de Estado que estao terminadas
%----------------------------------------------------------------------------

ilhas_terminadas(Estado,Ilhas_term) :- ilhas_terminadas(Estado,Ilhas_term,[]).
ilhas_terminadas([],Aux,Aux).
ilhas_terminadas([[ilha(NP,Cord),_,Pontes]|R],Ilhas_term,Aux) :-length(Pontes,X),(number(NP),X=:=NP, % verifica se o numero de pontes da ilha e o comprimento
                                                                append(Aux,[ilha(NP,Cord)],NovoAux), % da lista de pontes
                                                                ilhas_terminadas(R,Ilhas_term,NovoAux));
                                                                (ilhas_terminadas(R,Ilhas_term,Aux)).

%----------------------------------------------------------------------------
%    2.11 - tira_ilhas_terminadas_entrada(Ilhas_term,Entrada,Nova_Entrada)
%
%    Devolve Novo_Entrada, igual a Entrada so que com as ilhas terminadas retiradas da lista de viznhas 
%----------------------------------------------------------------------------

tira_ilhas_terminadas_aux(Ilhas_term,Viz,X) :- member(X,Ilhas_term), member(X,Viz). % funcao auxiliar que identifica se ilhas sao simultaneamente
                                                                                    % vizinhas e auxiliares
tira_ilhas_terminadas_entrada(Ilhas_term,Entrada,NovaEntrada) :- tira_ilhas_terminadas_entrada(Ilhas_term,Entrada,NovaEntrada,[]).
tira_ilhas_terminadas_entrada(_,[],Aux,Aux).
tira_ilhas_terminadas_entrada(Ilhas_term,[ilha(NP,Cord),Viz,Pontes],NovaEntrada,Aux) :- exclude(tira_ilhas_terminadas_aux(Ilhas_term,Viz),Viz,NViz), 
                                                                                        append(Aux,[ilha(NP,Cord),NViz,Pontes],NAux),
                                                                                        tira_ilhas_terminadas_entrada(_,[],NovaEntrada,NAux).

%----------------------------------------------------------------------------
%    2.12 - tira_ilhas_terminadas(Estado,Ilhas_term,Novo_Estado)
%
%    Devolve Novo_Estado, igual a Estado so que sem as ilhas terminadas na lista
%    de vizinhas de cada entrada de Estado
%----------------------------------------------------------------------------

tira_ilhas_terminadas(Estado,Ilhas_term,Novo_Estado) :- maplist(tira_ilhas_terminadas_entrada(Ilhas_term),Estado,Novo_Estado).

%---------------------------------------------------------------------------
%    2.13 - marca_ilhas_terminadas_entrada(Ilhas_term,Entrada,Nova_Entrada)
%
%    Devolve Nova_entrada, igual a Entrada so que com um X no lugar das Pontes
%    caso a ilhas esteja terminada
%----------------------------------------------------------------------------

marca_ilhas_terminadas_entrada(Ilhas_term,[ilha(NP,Cord),Viz,Pontes],[ilha('X',Cord),Viz,Pontes]) :- member(ilha(NP,Cord),Ilhas_term).
marca_ilhas_terminadas_entrada(Ilhas_term,[ilha(NP,Cord),Viz,Pontes],[ilha(NP,Cord),Viz,Pontes]) :- \+ member(ilha(NP,Cord),Ilhas_term).

%-------------------------------------------------------------------------
%    2.14 - marca_ilhas_terminadas(Estado,Ilhas_term,Novo_Estado)
%
%    Devolve Novo_Estado, igual a Estado so que com um X no lugar das Pontes
%    de todas as ilhas terminadas de Estado
%----------------------------------------------------------------------------

marca_ilhas_terminadas(Estado,Ilhas_term,Novo_Estado) :- maplist(marca_ilhas_terminadas_entrada(Ilhas_term),Estado,Novo_Estado).

%-------------------------------------------------------------------------
%    2.15 - trata_ilhas_terminadas(Estado,Novo_Estado)
%
%    Devolve Novo_Estado, resultado de aplicar marca_ilhas_terminadas e tira_ilhas_terminadas a Estado
%----------------------------------------------------------------------------

trata_ilhas_terminadas(Estado,Novo_Estado) :- ilhas_terminadas(Estado,Ilhas_term),tira_ilhas_terminadas(Estado,Ilhas_term,Novo_Estado1),
                                              marca_ilhas_terminadas(Novo_Estado1,Ilhas_term,Novo_Estado).

%-------------------------------------------------------------------------
%    2.16 - junta_pontes(Estado,Num_pontes,Ilha1,Ilha2,Novo_Estado)
%
%    Devolve Novo_Estado, resultado de criar uma ponte entre Ilha 1 e Ilha 2
%----------------------------------------------------------------------------
                                                                                                            
junta_pontes([[ilha(NP,CordAt),Viz,Pontes]|R],Num_pontes,Ilha1,Ilha2,Novo_Estado) :- junta_pontes([[ilha(NP,CordAt),Viz,Pontes]|R],Num_pontes,Ilha1,Ilha2,Novo_Estado, []).
junta_pontes([[ilha(NP,CordAt),Viz,Pontes]|R],Num_pontes,ilha(NP1,Cord1),ilha(NP2,Cord2),Novo_Estado,Aux) :-cria_ponte(Cord1,Cord2,Ponte),!,
                                                                                                            ((Num_pontes =:= 1,append([],[Ponte],AuxPonte));      % cria aux ponte conforme o numero de pontes a juntar
                                                                                                            (Num_pontes =:= 2,append([],[Ponte,Ponte],AuxPonte))),
                                                                                                            junta_pontes([[ilha(NP,CordAt),Viz,Pontes]|R],Num_pontes,ilha(NP1,Cord1),ilha(NP2,Cord2),Novo_Estado,Aux,AuxPonte).

junta_pontes([[ilha(NP,CordAt),Viz,Pontes]|R],Num_pontes,ilha(NP1,Cord1),ilha(NP2,Cord2),Novo_Estado,Aux,AuxPonte) :- 
                                                                                                            (ilha(NP,CordAt) = ilha(NP1,Cord1), % adiciona a ilha 1 a Aux, com lista de pontes atualizada
                                                                                                            append(Pontes,AuxPonte,PontesAT),
                                                                                                            append(Aux,[[ilha(NP,CordAt),Viz,PontesAT]],NovoAux),
                                                                                                            junta_pontes(R,Num_pontes,ilha(NP1,Cord1),ilha(NP2,Cord2),Novo_Estado,NovoAux,AuxPonte),!; 
                                                                                                            ilha(NP,CordAt) = ilha(NP2,Cord2),   % adiciona a ilha 2  a aux com a lista de pontes atualizada
                                                                                                            append(Pontes,AuxPonte,PontesAT),
                                                                                                            append(Aux,[[ilha(NP,CordAt),Viz,PontesAT]],NovoAux),
                                                                                                            junta_pontes(R,Num_pontes,ilha(NP1,Cord1),ilha(NP2,Cord2),Novo_Estado,NovoAux,AuxPonte),!;
                                                                                                            append(Aux,[[ilha(NP,CordAt),Viz,Pontes]],NAux), % adiciona as restantes ilhas a solucao
                                                                                                            junta_pontes(R,Num_pontes,ilha(NP1,Cord1),ilha(NP2,Cord2),Novo_Estado,NAux,AuxPonte)).

junta_pontes([],_,ilha(_,Cord1),ilha(_,Cord2),Novo_Estado,Aux,_) :- actualiza_vizinhas_apos_pontes(Aux,Cord1,Cord2,Res1),
                                                                    trata_ilhas_terminadas(Res1,Novo_Estado).