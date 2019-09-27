{%
    title = "Markdown jezik";
    author = {
        name: "Olivera Popović",
        id: "64/2016"
    };
    course = {
        name: "Konstrukcija kompilatora",
        short: "kk"
    };

    # Ovo je primer komentara
    
    faculty = "Matematički fakultet";
    markdown_authors = ["John Gruber", "Aaron Swartz"];
    services = ["GitHub", "GitLab", "BitBucket"];

    debug = True;
    version = 1.0;
%}

# {{title}}

## O seminarskom radu
- autor: {{author.name}} {{author.id}}
- kurs: {{course.name}} ({{course.short}})
- fakultet: {{faculty}}
{{ if debug }}
- verzija prezentacije: {{version}}
{{ endif }}

## Jezik Markdown
- Razvijen od strane autora {{markdown_authors[0]}} i {{markdown_authors[1]}}
- Inicijalno objavljen 2004. godine
- Trenutna verzija: {{version}}

## Servisi koji intenzivno koriste Markdown
{{ for service in services }}
- {{ service }}
{{ endfor }}

## Pitanja?

