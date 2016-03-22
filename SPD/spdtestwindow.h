#ifndef SPDTESTWINDOW_H
#define SPDTESTWINDOW_H

#include <QWidget>
#include <QLineEdit>
#include <QLabel>
#include <QPushButton>
#include <QListWidget>
#include <QDoubleSpinBox>
#include <QSpinBox>
#include <QCheckBox>
#include "SPDAnalysisModel.h"
#include "ftkGUI/GraphWindow.h"
#include "ftkGUI/HistoWindow.h"
#include "ftkGUI/PlotWindow.h"
#include "ProgressionHeatmapWindow.h"
#include "HeatmapWindow.h"

class SPDtestWindow : public QWidget
{
    Q_OBJECT

public:
    SPDtestWindow(QWidget *parent = 0);
	void setModels(vtkSmartPointer<vtkTable> table = NULL, ObjectSelection * sels = NULL, ObjectSelection * sels2 = NULL);
    ~SPDtestWindow();
	void GetProgressionTreeOrder(std::vector<long int> &order);
	vtkSmartPointer<vtkTable> NormalizeTable(vtkSmartPointer<vtkTable> table);

protected:
	void split(std::string& s, char delim, std::vector< unsigned int>& indexVec);
	void GetFeatureOrder(std::vector< unsigned int> &selID, std::vector< int> &selIdOrder, std::vector< int> &unselIdOrder);
	void showHeatmapAfterFeatureClustering();
	bool IsExist(std::vector< unsigned int> vec, unsigned int value);
	virtual void closeEvent(QCloseEvent *event);
	void closeSubWindows();
	vtkSmartPointer<vtkTable> GetSubTableExcludeItems( vtkSmartPointer<vtkTable> table, std::set<long int> &selItems);

protected slots:
    void browse();
    void load();
	void loadContrastData();
	void showOriginalHeatmap();
    void clusterFunction();
	void emdFunction();
	void showPSM();
	void viewProgression();
	//void saveSelectedFeatures();
	void updateSelMod();
	void editThreshold();
	//void editPercentage();
	void editNearestNeighbor();
	void showProgressionHeatmap();
	void regenerateProgressionTree();
	//void updateProgressionType();
	void ReRunSPDAnlysis();
	void ReColorProgressionTree(int nfeature);
	void UpdateConnectedNum();
	void searchSubsetsOfFeatures();

private:
	SPDAnalysisModel *SPDModel;
    QLabel *dataFileLabel;

    QLabel *dataFileName;
    QPushButton *browseButton;
    QPushButton *loadButton;
	//QPushButton *loadTestButton;
	
    QLabel *featureNumLabel;
    QLabel *featureNum;
    QLabel *sampleNumLabel;
    QLabel *sampleNum;

    QLabel *clusterCoherenceLabel;
    QDoubleSpinBox *clusterCoherenceBox;
    QLabel *kNearestNeighborLabel;
    QSpinBox *kNearestNeighborBox;
    //QPushButton *clusterButton;

	QLabel *emdLabel;
	//QLabel *progressionOverDistance;
	//QCheckBox *bcheckBox;   // progression overall or over distance to device
	QPushButton *emdButton;
	QDoubleSpinBox *emdThresBox;
	QLineEdit *emdPercentageBox;
	QLabel *psmLable;
	QLabel *psmPerLable;
    QPushButton *psmButton;
	QLabel *psdtLable;   // progression sample discovery tree
	QLineEdit *psdModuleSelectBox;  // select similar modules
	QLabel *maxVetexIdLabel;  // max id to seperate the data
	QSpinBox *maxVetexIdEdit; // max id
    QPushButton *psdtButton;
	QLabel *heatmapLabel;
	QPushButton *heatmapButton;  // show progression heatmap  // now shows the progression over distance to device
	QLabel *distanceLabel;
	QDoubleSpinBox *distanceThres;  // distance threshold for calculating percentage
	QLabel *connectedGraphLabel;
	QLineEdit *connectedGraphEdit;
	QPushButton *updateConnectedNumButton;  // update connected component number
	QPushButton *searchSubsetsButton;  // search subsets of features to best fit distance 
	
	QString FileName;
	GraphWindow *graph;
	ProgressionHeatmap *simHeatmap;
	HistoWindow *histo;
	Heatmap *originalHeatmap;
	Heatmap *progressionHeatmap;
	Heatmap *HeatmapWin;
	PlotWindow *plot;

	vnl_vector<int> optimalleaforder;

	vtkSmartPointer<vtkTable> data;
	std::set<long int> excludedIds;
	ObjectSelection *selection;  /** selection for threshold and IDs */
	ObjectSelection *selection2; 

	std::vector< unsigned int> selFeatureID;
	std::vector< int> selOrder;
	std::vector< int> unselOrder;
	vtkSmartPointer<vtkTable> tableAfterCellCluster;
	std::map< int, int> indexMap;
	std::vector<int> connectedComponent;
	int connectedNum;
	std::set<unsigned int> m_selModuleID;
	bool bconnected;
};

#endif // SPDTESTWINDOW_H
