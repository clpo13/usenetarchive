#include <QFileDialog>
#include <sstream>
#include <memory>

#include "../libuat/Archive.hpp"
#include "../common/String.hpp"

#include "browser.h"
#include "ui_browser.h"

Browser::Browser( QWidget *parent )
    : QMainWindow( parent )
    , ui( new Ui::Browser )
{
    ui->setupUi( this );
}

Browser::~Browser()
{
    delete ui;
}

void Browser::on_actionOpen_triggered()
{
    QFileDialog dialog;
    dialog.setFileMode( QFileDialog::Directory );
    dialog.setOption( QFileDialog::ShowDirsOnly );
    int res = dialog.exec();
    if( res )
    {
        std::string dir = dialog.selectedFiles()[0].toStdString();
        m_archive = std::make_unique<Archive>( dir );
        QString str;
        str += "Loaded archive with ";
        str += QString::number( m_archive->NumberOfMessages() );
        str += " messages.";
        ui->statusBar->showMessage( str, 0 );
        FillTree();
        auto idx = dir.find_last_of( '/' );
        ui->tabWidget->setTabText( 0, dir.substr( idx+1 ).c_str() );
    }
}

void Browser::FillTree()
{
    m_model = std::make_unique<TreeModel>( *m_archive );
    ui->treeView->setModel( m_model.get() );
    for( int i=0; i<4; i++ )
    {
        ui->treeView->resizeColumnToContents( i );
    }
    connect( ui->treeView->selectionModel(), SIGNAL( currentChanged( QModelIndex, QModelIndex ) ), this, SLOT( onTreeSelectionChanged( QModelIndex ) ) );
}

static void Encode( TextBuf& buf, const char* txt, const char* end )
{
    while( txt != end )
    {
        if( *txt == '<' )
        {
            buf.Write( "&lt;" );
        }
        else if( *txt == '>' )
        {
            buf.Write( "&gt;" );
        }
        else if( *txt == '*' )
        {
            const char* tmp = txt+1;
            for(;;)
            {
                if( tmp == end || *tmp == ' ' )
                {
                    tmp = end;
                    break;
                }
                if( *tmp == '*' )
                {
                    break;
                }
                tmp++;
            }
            if( tmp == end || tmp - txt == 1 )
            {
                buf.PutC( *txt );
            }
            else
            {
                buf.PutC( '*' );
                buf.Write( "<b>" );
                buf.Write( txt+1, tmp - txt - 1 );
                buf.Write( "</b>" );
                buf.PutC( '*' );
                txt = tmp;
            }
        }
        else if( *txt == '/' )
        {
            const char* tmp = txt+1;
            for(;;)
            {
                if( tmp == end || *tmp == ' ' )
                {
                    tmp = end;
                    break;
                }
                if( *tmp == '/' )
                {
                    break;
                }
                tmp++;
            }
            if( tmp == end || tmp - txt == 1 )
            {
                buf.PutC( *txt );
            }
            else
            {
                buf.PutC( '/' );
                buf.Write( "<i>" );
                buf.Write( txt+1, tmp - txt - 1 );
                buf.Write( "</i>" );
                buf.PutC( '/' );
                txt = tmp;
            }
        }
        else if( *txt == '_' )
        {
            const char* tmp = txt+1;
            for(;;)
            {
                if( tmp == end || *tmp == ' ' )
                {
                    tmp = end;
                    break;
                }
                if( *tmp == '_' )
                {
                    break;
                }
                tmp++;
            }
            if( tmp == end || tmp - txt == 1 )
            {
                buf.PutC( *txt );
            }
            else
            {
                buf.PutC( '_' );
                buf.Write( "<u>" );
                buf.Write( txt+1, tmp - txt - 1 );
                buf.Write( "</u>" );
                buf.PutC( '_' );
                txt = tmp;
            }
        }
        else
        {
            buf.PutC( *txt );
        }
        txt++;
    }
}

void Browser::SetText( const char* txt )
{
    m_buf.Reset();
    m_buf.Write( "<body><html><pre style=\"font-family: Consolas\"><p style=\"background-color: #1c1c1c\"><font color=\"#555555\">" );

    bool headers = true;
    bool first = true;
    bool sig = false;
    for(;;)
    {
        auto end = txt;
        while( *end != '\n' && *end != '\0' ) end++;
        if( headers )
        {
            if( end-txt == 0 )
            {
                m_buf.Write( "</font></p>" );
                headers = false;
                while( *end == '\n' ) end++;
                end--;
            }
            else
            {
                bool font = true;
                if( !first )
                {
                    m_buf.Write( "<br/>" );
                }
                first = false;
                if( strnicmpl( txt, "from: ", 6 ) == 0 )
                {
                    m_buf.Write( "<font color=\"#f6a200\">" );
                }
                else if( strnicmpl( txt, "newsgroups: ", 12 ) == 0 )
                {
                    m_buf.Write( "<font color=\"#0068f6\">" );
                }
                else if( strnicmpl( txt, "subject: ", 9 ) == 0 )
                {
                    m_buf.Write( "<font color=\"#74f600\">" );
                }
                else if( strnicmpl( txt, "date: ", 6 ) == 0 )
                {
                    m_buf.Write( "<font color=\"#f6002e\">" );
                }
                else
                {
                    font = false;
                }
                Encode( m_buf, txt, end );
                if( font )
                {
                    m_buf.Write( "</font>" );
                }
            }
        }
        else
        {
            bool font = true;
            if( strncmp( "-- \n", txt, 4 ) == 0 )
            {
                sig = true;
            }
            if( sig )
            {
                m_buf.Write( "<font color=\"#666666\">" );
            }
            else
            {
                int level = 0;
                auto test = txt;
                while( test != end )
                {
                    if( *test == '>' || *test == ':' || *test == '|' )
                    {
                        level++;
                    }
                    else if( *test != ' ' )
                    {
                        break;
                    }
                    test++;
                }
                switch( level )
                {
                case 0:
                    font = false;
                    break;
                case 1:
                    m_buf.Write( "<font color=\"#ae4a00\">" );
                    break;
                case 2:
                    m_buf.Write( "<font color=\"#980e76\">" );
                    break;
                case 3:
                    m_buf.Write( "<font color=\"#4e47ab\">" );
                    break;
                default:
                    m_buf.Write( "<font color=\"#225025\">" );
                    break;
                }
            }
            Encode( m_buf, txt, end );
            if( font )
            {
                m_buf.Write( "</font><br/>" );
            }
            else
            {
                m_buf.Write( "<br/>" );
            }
        }
        if( *end == '\0' ) break;
        txt = end + 1;
    }

    m_buf.Write( "</pre></html></body>" );
    ui->textBrowser->setHtml( QString( m_buf ) );
}

void Browser::on_treeView_clicked(const QModelIndex &index)
{
    auto idx = m_model->GetIdx( index );
    if( idx == -1 ) return;

    SetText( m_archive->GetMessage( idx ) );
}

void Browser::onTreeSelectionChanged( const QModelIndex& index )
{
    on_treeView_clicked( index );
}

void Browser::on_treeView_expanded(const QModelIndex &index)
{
    if( m_model->IsRoot( index ) )
    {
        RecursiveExpand(index);
    }
}

void Browser::RecursiveExpand(const QModelIndex& index)
{
    auto num = m_model->rowCount(index);
    for( int i=0; i<num; i++ )
    {
        auto idx = m_model->index( i, 0, index );
        ui->treeView->expand(idx);
        RecursiveExpand(idx);
    }
}
