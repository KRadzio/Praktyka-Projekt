DIR=./
TEXFILES=$(foreach D, $(DIR), $(wildcard $(D)*.tex))
PDFS=$(addsuffix .pdf, $(basename $(notdir $(TEXFILES))))
LATEXCOM=pdflatex

all: $(PDFS)

%.pdf:%.tex
	$(LATEXCOM) $<