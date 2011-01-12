/*
 * teste.java
 * 
 *
 * Created by Danilo Avila on 20/07/10.
 * Copyright 2010 __MyCompanyName__. All rights reserved.
 */




public class teste{

	public static String retornaNome(int naipe){
		if(naipe==0){
			return("Paus");
		}
		else if(naipe==1){
			return("Ouros");
		}
		else if(naipe==2){
			return("Copas");
		}
		else if(naipe==3){
			return("Espadas");
		}
		else if(naipe==4){
			return("Tales eu te falei");
		}
		
		return(null);
	}
	


	public static void main(String[] args){
	
		int naipe = 1;
		System.out.println(retornaNome(naipe));
		
	}

}

