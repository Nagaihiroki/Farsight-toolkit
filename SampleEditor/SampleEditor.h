/*=========================================================================
Copyright 2009 Rensselaer Polytechnic Institute
Licensed under the Apache License, Version 2.0 (the "License");
you may not use this file except in compliance with the License.
You may obtain a copy of the License at

http://www.apache.org/licenses/LICENSE-2.0

Unless required by applicable law or agreed to in writing, software
distributed under the License is distributed on an "AS IS" BASIS,
WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
See the License for the specific language governing permissions and
limitations under the License. 
=========================================================================*/

//**************************************************************************
// SampleEditor
//
// A simple tool to show how to use the ftkGUI classes for table viewing
// and manipulation.
// 
//**************************************************************************
#ifndef SAMPLE_EDITOR_H
#define SAMPLE_EDITOR_H

//QT Includes:
#include <QtGui/QMainWindow>
#include <QtGui/QMenuBar>
#include <QtGui/QMenu>
#include <QtGui/QMessageBox>
#include <QtGui/QFileDialog>
#include <QtGui/QTableView>
#include <QtGui/QDockWidget>
#include <QtCore/QFileInfo>

//VTK Includes:
#include <vtkTable.h>
#include <vtkSmartPointer.h>
#include <vtkVariantArray.h>
#include <vtkDoubleArray.h>

//Farsight Includes:
#include "ftkGUI/TableWindow.h"
#include "ftkGUI/PlotWindow.h"
#include "ftkGUI/HistoWindow.h"
#include "ftkGUI/ObjectSelection.h"
#include "ftkGUI/StatisticsToolbar.h"
#include "ftkGUI/SelectiveClustering.h"
#include "ftkUtils.h"
//#include "ftkGUI/GraphWindow.h"
#include "ftkGUI/GraphWindowForNewSelection.h"
#include "ClusClus/clusclus.h"
#include "ClusClus/Dendrogram.h"
#include "ClusClus/HeatmapWindow.h"
#include "ClusClus/Heatmap.h"
#include "ClusClus/LocalGeometryRef.h"
//#include "SPD/ProgressionHeatmapWindow.h"
//#include "SPD/spdmainwindow.h"
#include "SPD/SPDMSTModuleMatch.h"
#include "SPD/SPDkNNGModuleMatch.h"
#include <vector>
#include <string>

/**
* SampleEditor
* A simple tool to show how to use the ftkGUI classes for table viewing
* and manipulation.
*/ 
class SampleEditor : public QMainWindow
{
    Q_OBJECT;

public:
	SampleEditor(QWidget * parent = 0, Qt::WindowFlags flags = 0);

protected:
	void closeEvent(QCloseEvent *event);

private slots:
	void loadFile(void);
	void loadRotateFile(void);
	void removeRows(void);
	void addBlankRow(void);
	void changeRowData(void);
	void showStatistics(void);
	void updateStatistics(void);
	//void SPDAnalysis();
	void SPDMSTAnalysis();
	void SPDkNNGAnalysis();
	//void spdSampledendrogram();
	//void spdFeatureDendroram();
	//void spdShowHeatmap();

	void sampledendrogram();
	void featuredendrogram();
	void showheatmap();
	void biclusheatmap();
	void SpectralCluserting();

	void CreateClusterSelection();
	void DumpClusterSelections();
	void AddLabel();

signals:
    void selectionChanged(void);
private:
	void createMenus();
	void createStatusBar();
	void ReadFiles(std::string hname, std::string dname);
    QDockWidget *statisticsDockWidget;
	QMenu *fileMenu;
	QAction *loadAction;
	QAction *loadRotateAction;
	QMenu *editMenu;
	QAction *removeRowsAction;
	QAction *addBlankRowAction;
	QAction *changeRowDataAction;
	QAction *showStatisticsAction;
	QAction *updateStatisticsAction;


	//QAction *spdSampleDendroAction;
	//QAction *spdFeatureDendroAction;
	//QAction *spdHeatmapAction;

	QMenu *ClusClusMenu;
	QAction *sampleDendroAction;
	QAction *featureDendroAction;
	QAction *heatmapAction;

	QMenu *BiclusMenu;
	QAction *biclusHeatmapAction;

	QMenu *SpectralClusteringMenu;
	QAction *SpectralClusteringAction;

	QAction *CreateCluster;
	QAction *DisplayClusterSelections;

	StatisticsToolbar * statisticsToolbar;
	TableWindow *table;
	PlotWindow *plot;
	HistoWindow *histo;
	//GraphWindow *graph;
	Dendrogram *dendro1;
	Dendrogram *dendro2;
	Heatmap *heatmap;
	BiHeatmap *biheatmap;
	//ProgressionHeatmap *progressionheatmap;

	//SPDMainWindow *spdWin;
	QMenu *SPDMenu;
	QAction *SPDAction1;
	QAction *SPDAction2;
	QAction *AddLabelAction;
	SPDMSTModuleMatch *spdMSTWin;
	SPDkNNGModuleMatch *spdkNNGWin;
	int flag;

	vtkSmartPointer<vtkTable> data;
	ObjectSelection *selection;
	ObjectSelection *selection2;
	QString lastPath;

	clusclus *cc1;
	clusclus *cc2;
	Bicluster *bc;

	SelectiveClustering * ClusterSelections;
	ClusterManager * SampleClusterManager;
 };


#endif
