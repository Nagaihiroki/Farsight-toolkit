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

#include "TrainingDialog.h"

TrainingDialog::TrainingDialog(vtkSmartPointer<vtkTable> table, const char * trainColumn, QWidget *parent)
	: QDialog(parent)
{
	this->setWindowTitle(tr("Training"));

	QLabel *topLabel = new QLabel(tr("Please enter comma separated list of IDs: "));

	inputsLayout = new QVBoxLayout;

	addButton = new QPushButton(tr("Add Class"));
	connect(addButton, SIGNAL(clicked()), this, SLOT(addClass()));
	addButton->setDefault(false);
	addButton->setAutoDefault(false);
	delButton = new QPushButton(tr("Remove Class"));
	connect(delButton, SIGNAL(clicked()), this, SLOT(remClass()));
	delButton->setDefault(false);
	delButton->setAutoDefault(false);
	QHBoxLayout *bLayout = new QHBoxLayout;
	bLayout->addWidget(addButton);
	bLayout->addWidget(delButton);
	bLayout->addStretch(20);

	saveButton = new QPushButton(tr("Save Model"));
	connect(saveButton, SIGNAL(clicked()), this, SLOT(saveModel(void)));
	saveButton->setDefault(false);
	saveButton->setAutoDefault(false);
	
	loadButton = new QPushButton(tr("Load Model"));
	connect(loadButton, SIGNAL(clicked()), this, SLOT(loadModel()));
	loadButton->setDefault(false);
	loadButton->setAutoDefault(false);
	
	quitButton = new QPushButton(tr("Cancel"));
	connect(quitButton, SIGNAL(clicked()), this, SLOT(reject()));
	quitButton->setDefault(false);
	quitButton->setAutoDefault(false);
	doneButton = new QPushButton(tr("Done"));
	connect(doneButton, SIGNAL(clicked()), this, SLOT(accept()));
	doneButton->setDefault(false);
	doneButton->setAutoDefault(false);
	

	QHBoxLayout *endLayout = new QHBoxLayout;
	endLayout->addStretch(25);
	endLayout->addWidget(loadButton);
	endLayout->addWidget(saveButton);
	endLayout->addWidget(quitButton);
	endLayout->addWidget(doneButton);

	QVBoxLayout *masterLayout = new QVBoxLayout;
	masterLayout->addWidget(topLabel);
	masterLayout->addLayout(inputsLayout);
	masterLayout->addLayout(bLayout);
	masterLayout->addStretch(20);
	masterLayout->addLayout(endLayout);

	this->setLayout(masterLayout);

	this->addClass();

	m_table = table;
	columnForTraining = trainColumn;
}

void TrainingDialog::addClass(void)
{
	if(inputLabels.size() == 10)
		return;

	//Create input box
	QLabel *label = new QLabel("Class " + QString::number(inputValues.size() + 1) + ": ");
	inputLabels.push_back( label );

	QLineEdit *inVals = new QLineEdit();
	inVals->setMinimumWidth(200);
	inVals->setFocusPolicy(Qt::StrongFocus);
	inputValues.push_back( inVals );

	QHBoxLayout *layout = new QHBoxLayout;
	layout->addWidget( inputLabels.back() );
	layout->addWidget( inputValues.back() );
	layout->addStretch(1);
	iLayouts.push_back(layout);
	inputsLayout->addLayout( iLayouts.back() );

	for(int i=1; i<inputValues.size(); ++i)
		QWidget::setTabOrder(inputValues.at(i-1), inputValues.at(i));
	QWidget::setTabOrder(inputValues.back(), addButton);
	QWidget::setTabOrder(addButton,delButton);
	QWidget::setTabOrder(delButton,quitButton);
	QWidget::setTabOrder(quitButton,saveButton);
	QWidget::setTabOrder(saveButton, inputValues.front());

	inputValues.back()->setFocus();

}

void TrainingDialog::remClass(void)
{
	if(inputLabels.size() == 0)
		return;

	delete inputLabels.back();
	delete inputValues.back();

	inputLabels.remove(inputLabels.size()-1);
	inputValues.remove(inputValues.size()-1);

	delete iLayouts.back();
	iLayouts.remove(iLayouts.size()-1);

}


void TrainingDialog::saveModel(void)
{	
	
	QString filename = QFileDialog::getSaveFileName(this, tr("Save Model As..."),lastPath, tr("TEXT(*.xml)"));
	if(filename == "")
		return;
	lastPath = QFileInfo(filename).absolutePath();
	
	this->accept();
	
	if(training.size()<2)
	{
	QMessageBox::critical(this, tr("Oops"), tr("Please enter ids for atleast 2 classes to save the model"));
	this->show();
	return;
	}
	
	
	 //Load the model into a new table 
	 vtkSmartPointer<vtkTable> new_table = vtkSmartPointer<vtkTable>::New();
	 new_table->Initialize();
	 
	 
	 //Add columns to the model table
	 vtkSmartPointer<vtkVariantArray> model_data = vtkSmartPointer<vtkVariantArray>::New();
	 model_data = m_table->GetRow(1);	
	 for(int i =0;i<model_data->GetNumberOfValues();++i)
				{
					vtkSmartPointer<vtkDoubleArray> column = vtkSmartPointer<vtkDoubleArray>::New();
					column->SetName( m_table->GetColumnName(i) );
					new_table->AddColumn(column);
			  }	  
			  
				
					
	//Add only the rows which have been selected by the user					
	for(int row = 0; (int)row < m_table->GetNumberOfRows(); ++row)  
		{	
		  vtkSmartPointer<vtkVariantArray> model_data = vtkSmartPointer<vtkVariantArray>::New();
		  if(m_table->GetValueByName(row, columnForTraining)!=-1)
			   {	
				for(int i =0;i<m_table->GetNumberOfColumns();++i)
						{	
							model_data->InsertNextValue(m_table->GetValue(row,i));
						}
						new_table->InsertNextRow(model_data);
			   }
		 }
		
	//Write the model into an XML file.	
	bool ok = ftk::SaveTable(filename.toStdString(),new_table);
  if(!ok)
    {
    cerr << "problem writing model to " << filename.toStdString() << endl;
    }
}



void TrainingDialog::loadModel(void)
{	

	 QString fileName  = QFileDialog::getOpenFileName(this, "Select training model to open", lastPath,
									tr("TXT Files (*.xml)"));
		if(fileName == "") 
			return;
	 	lastPath = QFileInfo(fileName).absolutePath();
		model_table = ftk::LoadTable(fileName.toStdString());
		if(!model_table) return;
		//Append the data to the current table
		this->Append();
	
		//Exit:
	QDialog::accept();
				
}



//Update the features in this table whose names match (sets doFeat)
//Will creat new rows if necessary:
void TrainingDialog::Append()
{
		vtkSmartPointer<vtkDoubleArray> column = vtkSmartPointer<vtkDoubleArray>::New();
		column->SetName(columnForTraining );
		column->SetNumberOfValues( m_table->GetNumberOfRows() );
		column->FillComponent(0,-1);
		m_table->AddColumn(column);
				
		for(int row = 0; (int)row < m_table->GetNumberOfRows(); ++row)  
			{
				m_table->SetValueByName(row, columnForTraining, -1);
				break;
			}
		
		unsigned char maxrow = m_table->GetNumberOfRows();
		unsigned char maxrow2 = m_table->GetNumberOfRows();
		
		std::cout<<maxrow2<<std::endl;

		vtkSmartPointer<vtkVariantArray> model_data = vtkSmartPointer<vtkVariantArray>::New();

		for(int row = 0; (int)row < model_table->GetNumberOfRows(); ++row)  
			{
				model_data = model_table->GetRow(row);
				m_table->InsertNextRow(model_data); 
				m_table->SetValue(maxrow,0,maxrow+1);
				maxrow = maxrow + 1;
			}
		
		//vtkSmartPointer<vtkStringArray> discrim_column = vtkSmartPointer<vtkStringArray>::New();
		//discrim_column->SetName("Original/Model" );
		//discrim_column->SetNumberOfValues(m_table->GetNumberOfRows());
		//m_table->AddColumn(discrim_column);
		//
		//for(int row = 0; (int)row < m_table->GetNumberOfRows(); ++row)  
		//	{	
		//		std::cout<<"I was here"<<std::endl;
		//		if(row<maxrow2)
		//		{
		//			m_table->SetValueByName(row, "Original/Model", "Original");
		//			std::cout<<"I was here"<<std::endl;
		//		}
		//		if(row>=maxrow2)
		//		{
		//			m_table->SetValueByName(row, "Original/Model", "Model");
		//			std::cout<<"I was there"<<std::endl;
		//		}
		//	}

 		emit changedTable();
}



void TrainingDialog::accept(void)
{
	//Update the table:
	this->parseInputValues();
	this->updateTable();

	//Exit:
	QDialog::accept();
}

void TrainingDialog::parseInputValues(void)
{
	training.clear();

	
	for(int c=0; c<inputValues.size(); ++c)
	{
		std::set<int> ids;
		QString input = inputValues.at(c)->displayText();
		QStringList values = input.split(",");
		for(int i=0; i<values.size(); ++i)
		{
			QString str = values.at(i);
			int v = str.toInt();
			ids.insert( v );
		}
		training.push_back(ids);
	}
}

void TrainingDialog::updateTable(void)
{
	if(training.size() == 0)
		return;

	//If need to create a new column do so now:
	vtkAbstractArray * output = m_table->GetColumnByName(columnForTraining);
	
	if(output == 0)
	{
		vtkSmartPointer<vtkDoubleArray> column = vtkSmartPointer<vtkDoubleArray>::New();
		column->SetName( columnForTraining );
		column->SetNumberOfValues( m_table->GetNumberOfRows() );
		column->FillComponent(0,-1);
		m_table->AddColumn(column);
	}

	for(int row = 0; (int)row < m_table->GetNumberOfRows(); ++row)  
	{
		int id = m_table->GetValue(row,0).ToInt();
		for(int c=0; c<(int)training.size(); ++c)
		{
			if( training.at(c).find(id) != training.at(c).end() )
			{
				m_table->SetValueByName(row, columnForTraining, vtkVariant( c + 1 ));
				break;
			}
		}
	}
	emit changedTable();
}
