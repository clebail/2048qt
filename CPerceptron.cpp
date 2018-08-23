#include "CPerceptron.h"

CPerceptron::CPerceptron(const QList<int>& nbInputs, int nbNeurone) {
    QList<CNeurone *> layer;
    int i;

    for(i=0;i<nbNeurone;i++) {
        CNeurone *n = new CNeurone(nbInputs.at(i)+1);
        layer.append(n);
    }

    layers.append(layer);
}

CPerceptron::~CPerceptron(void) {
    int i;

    for(i=0;i<layers.size();i++) {
        int j;
        QList<CNeurone *> layer = layers.at(i);

        for(j=0;j<layer.size();j++) {
            delete layer.at(j);
        }

        layer.clear();
    }
    layers.clear();
}

void CPerceptron::init(void) {
    int i;

    for(i=0;i<layers.size();i++) {
        int j;

        for(j=0;j<layers.at(i).size();j++) {
            layers.at(i).at(j)->init();
        }
    }
}

QList<double> CPerceptron::eval(const QList<QList<double> >& inputs) {
    QList<double> resultat;
    QList<double> in;
    int i, j;

    for(i=0;i<layers.at(0).size();i++) {
        CNeurone *n = layers.at(0).at(i);
        resultat << n->eval(inputs.at(i));
    }
    in << resultat;

    for(i=1;i<layers.size();i++) {
        for(j=0;j<layers.at(i).size();j++) {
            CNeurone *n = layers.at(i).at(j);

            resultat.clear();
            resultat << n->eval(in);
            in.clear();
            in << resultat;
        }
    }

    return resultat;
}

void CPerceptron::addLayer(int nbNeurone) {
    int nbInput = layers.last().size();
    int i;
    QList<CNeurone *> layer;

    for(i=0;i<nbNeurone;i++) {
        CNeurone *n = new CNeurone(nbInput+1);
        layer.append(n);
    }

    layers.append(layer);
}

void CPerceptron::from(CPerceptron * p1, CPerceptron * p2) {
    int i;

    for(i=0;i<layers.size();i++) {
        int j;

        for(j=0;j<layers.at(i).size();j++) {
            CNeurone *n = layers.at(i).at(j);
            CNeurone *n1 = p1->layers.at(i).at(j);
            CNeurone *n2 = p2->layers.at(i).at(j);
            int seuil = rand() % n->getNbGene();

            n->from(n1, n2, seuil);
            n->mute(rand() % n->getNbGene());
        }
    }
}
