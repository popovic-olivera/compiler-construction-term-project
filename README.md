# Compiler construction term project

Ideja projekta je proširivanje markdown jezika mogućnošću korišćenja promenljivih, objekata i nizova.

# Način korišćenja

- Prvi deo koda, ograničen sa {% i %}, služi za deklaracije i definicije.

- Podržani tipovi promenljivih su realni brojevi, stringovi i logički tip. Pored promenljivih, moguće je i korišćenje objekata i nizova. Komentari se navode početnim znakom #. ( U primeru **example.md** prikazana je sintaksa za ove definicije )

- Nakon prvog dela sledi tekst napisan po pravilima za markdown, sa dopunama:
  1. Vrednost promenljive dobija se pozivom: **{{id}}**
  2. Vrednost atributa objekta dobija se pozivom: **{{id_objekta.id_atributa}}** 
  3. Vrednost elementa niza dobija se pozivom: **{{id_niza[indeks]}}** 
  
- Vrednost se upisuje upravo na to mesto nakon obrade.

- Postoji mogućnost korišćenja if naredbe. U zavisnosti od logičke promenljive x tekst ograničen sa **{{ if x }}** i **{{ endif }}** će se ispisati ili neće. Tekst između može sadržati vrednosti svih prethodno definisanih promenljivih, elemenata niza i atributa objekata.

- Takođe, postoji mogućnost korišćenja naredbe for. Naredbom for moguće je kretati se kroz niz, a tekst ograničen sa **{{ for x in xs }}** i **{{ endfor }}** ispisaće se onoliko puta koliko ima elemenata u nizu. Tekst između može sadržati vrednosti svih prethodno definisanih promenljivih, elemenata niza i atributa objekata, kao i vrednost nove uvedene promenljive x.

- Greške koje se javljaju u programu obojene su različitim bojama. Takođe pri javljanju greške prikazuje se i linija izvornog koda u kojoj se greška desila.

- Program se poziva navodjenjem **.md** fajla koji se obrađuje, kao i inavođenjem imena fajla koji će sadržati rezultat obrade.

- Rezultat obrade je funkcionalan markdown kod, koji se dalje može menjati i dopunjavati :)
