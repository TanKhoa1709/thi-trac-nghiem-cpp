# Exam Management System (CTDL&GT)

## Overview
A Qt-based exam management system for multiple choice tests with student and teacher roles.

## Core Features
- **Authentication**: Student/Teacher login system
- **Subject Management**: CRUD operations for subjects
- **Question Management**: Binary search tree for questions
- **Class Management**: Array-based class storage with linked student lists
- **Student Management**: Linked list implementation
- **Exam System**: Random question selection and scoring
- **Score Tracking**: Detailed exam results and history

## Data Structures
- **Subjects**: Linear list with BST for questions
- **Questions**: Binary search tree by ID
- **Classes**: Array of pointers to student lists
- **Students**: Linked list with exam scores
- **Scores**: Linked list with detailed results

## Build Requirements
- Qt6 Widgets
- Qt6 Core
- Qt6 PrintSupport
- C++17 compiler

## Usage
1. Login as teacher (full access) or student (exam only)
2. Teachers can manage subjects, questions, classes, and students
3. Students can take exams and view their results
4. System supports file-based data persistence


