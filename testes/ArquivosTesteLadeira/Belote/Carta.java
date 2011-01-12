/*
 * Aluno: Daniel Saad Nogueira Nunes
 * Matrícula: 06/81792
 * Disciplina: Sistemas De Programação
 * Professor: Tiago Trindade
 * Curso: Ciência da Computação
 */
public class Carta {
	public char naipe; /*0,1,2,3 respectivamente paus,ouros,espadas,copas*/
	public String nome;/*Nome da carta*/
	public int valor;/*Valor da carta, As = 1, dois = 2,...,etc.*/
	public Carta(char value,String str,int type){
		naipe = value;
		nome = str;
		valor = type;
	}
	public void printOut(){ /*metodo para impressao da carta*/
		String strnaipe = new String();
		if(naipe=='0'){
			strnaipe="Paus";
		}
		else if(naipe=='1'){
			strnaipe="Ouros";
		}
		else if(naipe=='2'){
			strnaipe ="Copas";
		}
		else if(naipe=='3'){
			strnaipe ="Espadas";
		}else if (naipe=='4') {
			strnaipe="tales gay";
		}
		System.out.print("O Naipe da carta e: ");
		System.out.println(strnaipe);
		//System.out.println("\n");
		System.out.print("O valor da carta e: ");
		System.out.println(valor);
		//System.out.println("\n");
		System.out.print("O nome da carta e: ");
		System.out.println(nome);
		System.out.println("");
		return;
	}
}
