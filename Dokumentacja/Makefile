DIR=./
TEXFILES=$(foreach D, $(DIR), $(wildcard $(D)/*.tex))
LATEXCOM=pdflatex

all: $(TEXFILES)
	$(LATEXCOM) $<
